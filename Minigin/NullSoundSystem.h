#pragma once
#include "BaseSoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		~NullSoundSystem() = default;
		
		void PlaySound(SoundId, float) {};
		void PauseSound() {};
		void ResumeSound() {};
		void StopSound() {};

		void PlayMusic(SoundId, float, int) {};
		void PauseMusic() {};
		void ResumeMusic() {};
		void StopMusic() {};

		void RegisterSound(SoundId, const std::string&) {};
		void RegisterMusic(SoundId, const std::string&) {};
	};
}
