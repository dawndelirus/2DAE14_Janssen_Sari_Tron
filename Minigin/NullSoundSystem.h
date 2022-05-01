#pragma once
#include "SoundSystemBase.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystemBase
	{
	public:
		void Play() override {};
		void Pause() override {};
		void Stop() override {};
	};
}
