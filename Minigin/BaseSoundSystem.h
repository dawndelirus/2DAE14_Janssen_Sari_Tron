#pragma once

namespace dae
{
	using SoundId = unsigned short;
	class BaseSoundSystem
	{
	public:
		BaseSoundSystem() = default;
		virtual ~BaseSoundSystem() = default;

		BaseSoundSystem(const BaseSoundSystem&) = delete;
		BaseSoundSystem(BaseSoundSystem&&) = delete;
		BaseSoundSystem& operator= (const BaseSoundSystem&) = delete;
		BaseSoundSystem& operator= (const BaseSoundSystem&&) = delete;

		virtual void Play(SoundId id, float volume) = 0;
		virtual void Pause(SoundId id) = 0;
		virtual void Stop(SoundId id) = 0;
		virtual void Resume(dae::SoundId id) = 0;

		virtual void RegisterSound(SoundId id, const std::string& path) = 0;
	};
}
