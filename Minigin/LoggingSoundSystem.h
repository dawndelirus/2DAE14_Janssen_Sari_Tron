#pragma once
#include "BaseSoundSystem.h"

namespace dae
{
	class LoggingSoundSystem : public BaseSoundSystem
	{
	public:
		LoggingSoundSystem(BaseSoundSystem* pRealSoundSystem);
		~LoggingSoundSystem();

		void Play(SoundId ud, float volume) override;
		void Pause(SoundId id) override;
		void Stop(SoundId id) override;
		void Resume(dae::SoundId id) override;

		virtual void RegisterSound(SoundId id, const std::string& path) override;
	private:
		BaseSoundSystem* m_pRealSoundSystem;
	};
}
