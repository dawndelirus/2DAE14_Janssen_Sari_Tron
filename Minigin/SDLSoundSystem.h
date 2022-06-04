#pragma once
#include "BaseSoundSystem.h"

class SDLSoundSystem final : public dae::BaseSoundSystem
{
public:
	SDLSoundSystem();
	~SDLSoundSystem();

	void PlaySound(dae::SoundId id, float volume) override;
	void PauseSound() override;
	void ResumeSound() override;
	void StopSound() override;

	void PlayMusic(dae::MusicId id, float volume, int loops) override;
	void PauseMusic() override;
	void ResumeMusic() override;
	void StopMusic() override;

	void RegisterSound(dae::SoundId id, const std::string& path) override;
	void RegisterMusic(dae::MusicId id, const std::string& path) override;

private:
	class SDLSoundSystemImpl;
	SDLSoundSystemImpl* m_pImpl;
};


