#pragma once

namespace dae
{
	class SoundSystemBase
	{
	public:
		SoundSystemBase() = default;
		virtual ~SoundSystemBase() = default;

		SoundSystemBase(const SoundSystemBase&) = delete;
		SoundSystemBase(SoundSystemBase&&) = delete;
		SoundSystemBase& operator= (const SoundSystemBase&) = delete;
		SoundSystemBase& operator= (const SoundSystemBase&&) = delete;

		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
	};
}
