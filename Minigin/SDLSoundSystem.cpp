#include "MiniginPCH.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

using namespace dae;

SDLSoundSystem::SDLSoundSystem()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096);
	Mix_ChannelFinished(ChannelDone);
}

SDLSoundSystem::~SDLSoundSystem()
{
	for (auto& chunks : m_SoundChunks)
	{
		delete chunks.second;
	}

	Mix_CloseAudio();
}

void SDLSoundSystem::Play(SoundId id, float volume)
{
	std::scoped_lock lock(m_SoundLock);

	Mix_Chunk* sound;

	auto itChunks = std::find(m_SoundChunks.begin(), m_SoundChunks.end(), id);
	if (itChunks == m_SoundChunks.end())
	{
		auto it = std::find(m_SoundPaths.begin(), m_SoundPaths.end(), id);
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

	sound->volume = volume;
	int channel = Mix_PlayChannel(-1, sound, 0);
	if (channel == -1)
	{
		std::cout << "Could not play sound with id: " << id << "\n";
		return;
	}

	m_SoundChannels.emplace(std::make_pair(id, channel));
}

void SDLSoundSystem::Pause(SoundId id)
{
	std::scoped_lock lock(m_SoundLock);
	int channel = GetChannel(id);
	Mix_Pause(channel);
}

void SDLSoundSystem::Stop(SoundId id)
{
	std::scoped_lock lock(m_SoundLock);
	int channel = GetChannel(id);
	Mix_HaltChannel(channel);
}

void SDLSoundSystem::Resume(dae::SoundId id)
{
	std::scoped_lock lock(m_SoundLock);
	int channel = GetChannel(id);
	Mix_Resume(channel);
}

void SDLSoundSystem::RegisterSound(SoundId id, const std::string& path)
{
	std::scoped_lock lock(m_SoundLock);
	m_SoundPaths.emplace(std::make_pair(id, path));
}

int SDLSoundSystem::GetChannel(dae::SoundId id)
{
	auto itChannels = std::find(m_SoundChannels.begin(), m_SoundChannels.end(), id);
	if (itChannels == m_SoundChannels.end())
	{
		std::cout << "Could not find channel playing sound with id: " << id << "\n";
		return -1;
	}
	return itChannels->second;
}

void ChannelDone(int channel)
{
	auto soundSystem = dynamic_cast<SDLSoundSystem*>(&ServiceLocator::GetSoundSystem());
	if (soundSystem == nullptr)
	{
		std::cout << "Current sound system is not SDL sound system! \n";
		return;
	}

	std::scoped_lock lock(soundSystem->m_SoundLock);
	
	auto soundChannels = soundSystem->m_SoundChannels;
	for (size_t i = 0; i < soundChannels.size(); i++)
	{
		if (soundChannels[i] == channel)
		{
			soundChannels.erase(i);
			return;
		}
	}
}
