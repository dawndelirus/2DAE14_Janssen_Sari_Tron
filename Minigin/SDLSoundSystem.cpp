#include "MiniginPCH.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include <mutex>
#include <map>
#include "SDL_mixer.h"

using namespace dae;

class SDLSoundSystem::SDLSoundSystemImpl
{
public:
	SDLSoundSystemImpl()
	{
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096);
	}

	~SDLSoundSystemImpl()
	{
		for (auto& chunks : m_SoundChunks)
		{
			Mix_FreeChunk(chunks.second);
		}

		Mix_CloseAudio();
	}

	void PlaySound(dae::SoundId id, float volume)
	{
		Mix_Chunk* sound = nullptr;

		auto itChunks = m_SoundChunks.find(id);

		if (itChunks == m_SoundChunks.end())
		{
			auto it = m_SoundPaths.find(id);
			if (it == m_SoundPaths.end())
			{
				return;
			}

			auto newSound = Mix_LoadWAV(it->second.c_str());
			if (newSound == NULL)
			{
				std::cout << "Couldn't load sound with path: " << it->second << "\n";
				return;
			}

			sound = newSound;
			m_SoundChunks.emplace(std::make_pair(id, sound));
		}
		else
		{
			sound = itChunks->second;
		}

		int channel = Mix_PlayChannel(-1, sound, 0);
		if (channel == -1)
		{
			std::cout << "Could not play sound with id: " << id << "\n";
			return;
		}

		int soundvolume = static_cast<int>(volume * static_cast<float>(SDL_MIX_MAXVOLUME));

		Mix_Volume(channel, soundvolume);
	}

	void PauseSound()
	{
		Mix_Pause(-1);
	}

	void ResumeSound()
	{
		Mix_Resume(-1);
	}

	void StopSound()
	{
		Mix_HaltChannel(-1);
	}

	void PlayMusic(dae::SoundId id, float volume, int loops)
	{
		Mix_Music* music = nullptr;

		auto itMusic = m_Music.find(id);

		if (itMusic == m_Music.end())
		{
			auto it = m_MusicPaths.find(id);
			if (it == m_MusicPaths.end())
			{
				return;
			}

			auto newMusic = Mix_LoadMUS(it->second.c_str());

			if (newMusic == NULL)
			{
				std::cout << "Couldn't load MUSIC with path: " << it->second << "\n";
				return;
			}

			music = newMusic;
			m_Music.emplace(std::make_pair(id, newMusic));
		}
		else
		{
			music = itMusic->second;
		}

		int soundvolume = static_cast<int>(volume * static_cast<float>(SDL_MIX_MAXVOLUME));
		Mix_VolumeMusic(soundvolume);
		Mix_PlayMusic(music, loops);
	}

	void PauseMusic()
	{
		Mix_PauseMusic();
	}

	void ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void StopMusic()
	{
		Mix_HaltMusic();
	}

	void RegisterSound(dae::SoundId id, const std::string& path)
	{
		auto itChunks = m_SoundChunks.find(id);

		if (itChunks == m_SoundChunks.end())
		{
			m_SoundPaths.emplace(std::make_pair(id, path));
		}
		else
		{
			std::stringstream messageStream;
			messageStream << "Sound with id: " << id << " already exists\n";
			std::string message = messageStream.str();
			std::cout << message;
		}
	}

	void RegisterMusic(dae::SoundId id, const std::string& path)
	{
		auto itMusic = m_Music.find(id);
		if (itMusic == m_Music.end())
		{
			m_MusicPaths.emplace(std::make_pair(id, path));
		}
		else
		{
			std::stringstream messageStream;
			messageStream << "Music with id: " << id << " already exists\n";
			std::string message = messageStream.str();
			std::cout << message;
		}
	}

private:
	std::map<dae::SoundId, Mix_Chunk*> m_SoundChunks;
	std::map<dae::SoundId, Mix_Music*> m_Music;
	std::map<dae::SoundId, std::string> m_SoundPaths;
	std::map<dae::SoundId, std::string> m_MusicPaths;
};

SDLSoundSystem::SDLSoundSystem()
{
	m_pImpl = new SDLSoundSystemImpl();
}

SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImpl;
}

void SDLSoundSystem::PlaySound(SoundId id, float volume)
{
	m_pImpl->PlaySound(id, volume);
}

void SDLSoundSystem::PauseSound()
{
	m_pImpl->PauseSound();
}

void SDLSoundSystem::ResumeSound()
{
	m_pImpl->ResumeSound();
}

void SDLSoundSystem::StopSound()
{
	m_pImpl->StopSound();
}

void SDLSoundSystem::PlayMusic(SoundId id, float volume, int loops)
{
	m_pImpl->PlayMusic(id, volume, loops);
}

void SDLSoundSystem::PauseMusic()
{
	m_pImpl->PauseMusic();
}

void SDLSoundSystem::ResumeMusic()
{
	m_pImpl->ResumeMusic();
}

void SDLSoundSystem::StopMusic()
{
	m_pImpl->StopMusic();
}

void SDLSoundSystem::RegisterSound(SoundId id, const std::string& path)
{
	m_pImpl->RegisterSound(id, path);
}

void SDLSoundSystem::RegisterMusic(SoundId id, const std::string& path)
{
	m_pImpl->RegisterMusic(id, path);
}
