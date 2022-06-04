#include "MiniginPCH.h"
#include "LoggingSoundSystem.h"

dae::LoggingSoundSystem::LoggingSoundSystem(BaseSoundSystem* pRealSoundSystem)
	: m_pRealSoundSystem{pRealSoundSystem}
{
}

dae::LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSoundSystem;
}

void dae::LoggingSoundSystem::PlaySound(SoundId id, float volume)
{
	m_pRealSoundSystem->PlaySound(id, volume);
	
	std::stringstream messageStream;
	messageStream << "Playing Sound: " << id << " at volume: " << volume << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::PauseSound()
{
	m_pRealSoundSystem->PauseSound();

	std::stringstream messageStream;
	messageStream << "Pausing: all sounds\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::StopSound()
{
	m_pRealSoundSystem->StopSound();

	std::stringstream messageStream;
	messageStream << "Stopping: all sounds\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::ResumeSound()
{
	m_pRealSoundSystem->ResumeSound();

	std::stringstream messageStream;
	messageStream << "Resuming: all sounds\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::PlayMusic(SoundId id, float volume, int loops)
{
	m_pRealSoundSystem->PlayMusic(id, volume, loops);

	std::stringstream messageStream;
	messageStream << "Playing music: " << id << " at volume: " << volume << " with " << loops << " loops\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::PauseMusic()
{
	m_pRealSoundSystem->PauseMusic();

	std::stringstream messageStream;
	messageStream << "Pausing music\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::ResumeMusic()
{
	m_pRealSoundSystem->ResumeMusic();

	std::stringstream messageStream;
	messageStream << "Resuming music\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::StopMusic()
{
	m_pRealSoundSystem->StopMusic();

	std::stringstream messageStream;
	messageStream << "Stopping music\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::RegisterSound(SoundId id, const std::string& path)
{
	m_pRealSoundSystem->RegisterSound(id, path);

	std::stringstream messageStream;
	messageStream << "Registered sound: " << id << " with path: " << path << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::RegisterMusic(SoundId id, const std::string& path)
{
	m_pRealSoundSystem->RegisterMusic(id, path);

	std::stringstream messageStream;
	messageStream << "Registered music: " << id << " with path: " << path << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}
