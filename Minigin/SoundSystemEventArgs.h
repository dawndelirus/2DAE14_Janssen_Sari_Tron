#pragma once
#include "EventArgs.h"
#include "BaseSoundSystem.h"

namespace dae
{
	class SoundEffectsEventArgs : public EventArgs
	{
	public:
		SoundEffectsEventArgs(SoundId i, float v)
			: id{i}, volume{v} {}
		virtual ~SoundEffectsEventArgs() = default;
		SoundId id;
		float volume;
	};

	class MusicEventArgs : public EventArgs
	{
	public:
		MusicEventArgs(MusicId i, float v, int l)
			: id{ i }, volume{ v }, loops{l} {}
		virtual ~MusicEventArgs() = default;
		MusicId id;
		float volume;
		int loops;
	};
}