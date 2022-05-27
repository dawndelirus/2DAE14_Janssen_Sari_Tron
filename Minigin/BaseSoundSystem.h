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

		virtual void PlaySound(SoundId id, float volume) = 0;
		virtual void PauseSound() = 0;
		virtual void ResumeSound() = 0;
		virtual void StopSound() = 0;

		virtual void PlayMusic(SoundId id, float volume, int loops) = 0;
		virtual void PauseMusic() = 0;
		virtual void ResumeMusic() = 0;
		virtual void StopMusic() = 0;

		virtual void RegisterSound(SoundId id, const std::string& path) = 0;
		virtual void RegisterMusic(SoundId id, const std::string& path) = 0;
	};
}
