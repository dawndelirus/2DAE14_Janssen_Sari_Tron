#pragma once
#include "BaseSoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		~NullSoundSystem() = default;

		void Play(SoundId, float) override {};
		void Pause(SoundId id) override {};
		void Stop(SoundId id) override {};
		void Resume(dae::SoundId id) override {};

		virtual void RegisterSound(SoundId, const std::string&) override {};
	};
}
