#pragma once
#include "BaseSoundSystem.h"

namespace dae
{
	class LoggingSoundSystem : public BaseSoundSystem
	{
	public:
		LoggingSoundSystem(BaseSoundSystem* pRealSoundSystem);
		~LoggingSoundSystem();

		void PlaySound(SoundId id, float volume) override;
		void PauseSound() override;
		void ResumeSound() override;
		void StopSound() override;

		void PlayMusic(SoundId id, float volume, int loops) override;
		void PauseMusic() override;
		void ResumeMusic() override;
		void StopMusic() override;

		void RegisterSound(SoundId id, const std::string& path) override;
		void RegisterMusic(SoundId id, const std::string& path) override;
	private:
		BaseSoundSystem* m_pRealSoundSystem;
	};
}
