#include "MiniginPCH.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"
#include "SoundSystemEventArgs.h"

#include <mutex>
#include <map>
#include <deque>
#include "SDL_mixer.h"

using namespace dae;

enum class Event
{
	PlaySound,
	PauseSound,
	ResumeSound,
	StopSound,

	PlayMusic,
	PauseMusic,
	ResumeMusic,
	StopMusic
};

class SDLSoundSystem::SDLSoundSystemImpl
{
public:
	
	SDLSoundSystemImpl();
	~SDLSoundSystemImpl();
	
	void AddToQueue(Event soundEvent, std::shared_ptr<EventArgs> pArgs);

	void PlaySound_T(dae::SoundId id, float volume);
	void PauseSound_T();
	void ResumeSound_T();
	void StopSound_T();

	void PlayMusic_T(dae::MusicId id, float volume, int loops);
	void PauseMusic_T();
	void ResumeMusic_T();
	void StopMusic_T();

	void RegisterSound(dae::SoundId id, const std::string& path);
	void RegisterMusic(dae::MusicId id, const std::string& path);

private:
	void UpdateThread_T();
	void ProcessEvent_T(Event event, std::shared_ptr<EventArgs> eventArgs);

	std::map<dae::SoundId, Mix_Chunk*> m_SoundChunks;
	std::map<dae::SoundId, Mix_Music*> m_Music;
	std::map<dae::SoundId, std::string> m_SoundPaths;
	std::map<dae::SoundId, std::string> m_MusicPaths;

	std::atomic<bool> m_IsThreadActive;
	std::deque<std::pair<Event, std::shared_ptr<EventArgs>>> m_SoundQueue;

	std::mutex m_QueueMutex;
	std::jthread m_Thread;
	std::condition_variable m_ConditionVariable;
};

SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
	:m_IsThreadActive{true}
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096);
	m_Thread = std::jthread(&SDLSoundSystem::SDLSoundSystemImpl::UpdateThread_T, this);
}

SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	{
		std::scoped_lock lock(m_QueueMutex);
		m_IsThreadActive = false;
	}
	m_ConditionVariable.notify_all();

	for (auto& chunks : m_SoundChunks)
	{
		Mix_FreeChunk(chunks.second);
	}

	for (auto& music : m_Music)
	{
		Mix_FreeMusic(music.second);
	}

	Mix_CloseAudio();
}

void SDLSoundSystem::SDLSoundSystemImpl::AddToQueue(Event soundEvent, std::shared_ptr<EventArgs> pArgs)
{
	{
		std::scoped_lock lock(m_QueueMutex);
		m_SoundQueue.emplace_back(std::make_pair(soundEvent, pArgs));
	}
	m_ConditionVariable.notify_all();
}

void SDLSoundSystem::SDLSoundSystemImpl::PlaySound_T(dae::SoundId id, float volume)
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

void SDLSoundSystem::SDLSoundSystemImpl::PauseSound_T()
{
	Mix_Pause(-1);
}

void SDLSoundSystem::SDLSoundSystemImpl::ResumeSound_T()
{
	Mix_Resume(-1);
}

void SDLSoundSystem::SDLSoundSystemImpl::StopSound_T()
{
	Mix_HaltChannel(-1);
}

void SDLSoundSystem::SDLSoundSystemImpl::PlayMusic_T(dae::MusicId id, float volume, int loops)
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

void SDLSoundSystem::SDLSoundSystemImpl::PauseMusic_T()
{
	Mix_PauseMusic();
}

void SDLSoundSystem::SDLSoundSystemImpl::ResumeMusic_T()
{
	Mix_ResumeMusic();
}

void SDLSoundSystem::SDLSoundSystemImpl::StopMusic_T()
{
	Mix_HaltMusic();
}

void SDLSoundSystem::SDLSoundSystemImpl::RegisterSound(dae::SoundId id, const std::string& path)
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

void SDLSoundSystem::SDLSoundSystemImpl::RegisterMusic(dae::MusicId id, const std::string& path)
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

void SDLSoundSystem::SDLSoundSystemImpl::UpdateThread_T()
{
	while (m_IsThreadActive)
	{
		std::unique_lock lock(m_QueueMutex);
		m_ConditionVariable.wait(lock, [this]
			{
				return !m_SoundQueue.empty() || !m_IsThreadActive;
			});

		std::pair<Event, std::shared_ptr<EventArgs>> soundEvent{};
		while (!m_SoundQueue.empty())
		{
			soundEvent = m_SoundQueue.front();
			m_SoundQueue.pop_front();
			lock.unlock();
			ProcessEvent_T(soundEvent.first, soundEvent.second);
			lock.lock();
		}
		lock.unlock();
	}
}

void SDLSoundSystem::SDLSoundSystemImpl::ProcessEvent_T(Event e, std::shared_ptr<EventArgs> ea)
{
	switch (e)
	{
	case Event::PlaySound:
	{
		auto args = std::dynamic_pointer_cast<SoundEffectsEventArgs>(ea);
		PlaySound_T(args->id, args->volume);
		break;
	}
	case Event::PauseSound:
		PauseSound_T();
		break;
	case Event::ResumeSound:
		ResumeSound_T();
		break;
	case Event::StopSound:
		StopSound_T();
		break;
	case Event::PlayMusic:
	{
		auto args = std::dynamic_pointer_cast<MusicEventArgs>(ea);
		PlayMusic_T(args->id, args->volume, args->loops);
		break;
	}
	case Event::PauseMusic:
		PauseMusic_T();
		break;
	case Event::ResumeMusic:
		ResumeMusic_T();
		break;
	case Event::StopMusic:
		StopMusic_T();
		break;
	default:
		break;
	}
}

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
	m_pImpl->AddToQueue(Event::PlaySound, std::make_shared<SoundEffectsEventArgs>(id, volume));
}

void SDLSoundSystem::PauseSound()
{
	m_pImpl->AddToQueue(Event::PauseSound, nullptr);
}

void SDLSoundSystem::ResumeSound()
{
	m_pImpl->AddToQueue(Event::ResumeSound, nullptr);
}

void SDLSoundSystem::StopSound()
{
	m_pImpl->AddToQueue(Event::StopSound, nullptr);
}

void SDLSoundSystem::PlayMusic(MusicId id, float volume, int loops)
{
	m_pImpl->AddToQueue(Event::PlayMusic, std::make_shared<MusicEventArgs>(id, volume, loops));
}

void SDLSoundSystem::PauseMusic()
{
	m_pImpl->AddToQueue(Event::PauseMusic, nullptr);
}

void SDLSoundSystem::ResumeMusic()
{
	m_pImpl->AddToQueue(Event::ResumeMusic, nullptr);
}

void SDLSoundSystem::StopMusic()
{
	m_pImpl->AddToQueue(Event::StopMusic, nullptr);
}

void SDLSoundSystem::RegisterSound(SoundId id, const std::string& path)
{
	m_pImpl->RegisterSound(id, path);
}

void SDLSoundSystem::RegisterMusic(MusicId id, const std::string& path)
{
	m_pImpl->RegisterMusic(id, path);
}
