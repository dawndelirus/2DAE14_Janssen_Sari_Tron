#pragma once
#include "BaseSoundSystem.h"
#include <map>
#include <mutex>
#include "SDL_mixer.h"

class SDLSoundSystem final : public dae::BaseSoundSystem
{
public:
	SDLSoundSystem();
	~SDLSoundSystem();
	void Play(dae::SoundId id, float volume) override;
	void Pause(dae::SoundId id) override;
	void Stop(dae::SoundId id) override;
	void Resume(dae::SoundId id) override;

	void RegisterSound(dae::SoundId, const std::string&) override;

private:
	friend void ChannelDone(int channel);

	int GetChannel(dae::SoundId id);

	std::map<dae::SoundId, std::string> m_SoundPaths;
	std::map<dae::SoundId, int> m_SoundChannels;
	std::map<dae::SoundId, Mix_Chunk*> m_SoundChunks;

	std::mutex m_SoundLock;
};


