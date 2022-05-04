#pragma once
#include "BaseSoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		~NullSoundSystem() = default;

		void Play(SoundId, float) override {};
		void Pause(SoundId) override {};
		void Stop(SoundId) override {};
		void Resume(dae::SoundId) override {};

		virtual void RegisterSound(SoundId, const std::string&) override {};
	};
}
