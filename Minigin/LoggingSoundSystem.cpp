#include "MiniginPCH.h"
#include "LoggingSoundSystem.h"

dae::LoggingSoundSystem::LoggingSoundSystem(BaseSoundSystem* pRealSoundSystem)
	: m_pRealSoundSystem{pRealSoundSystem}
{
}

dae::LoggingSoundSystem::~LoggingSoundSystem()
{
}

void dae::LoggingSoundSystem::Play(SoundId id, float volume)
{
	m_pRealSoundSystem->Play(id, volume);
	
	std::stringstream messageStream;
	messageStream << "Playing: " << id << " at volume: " << volume << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::Pause(SoundId id)
{
	m_pRealSoundSystem->Pause(id);

	std::stringstream messageStream;
	messageStream << "Pausing: " << id << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::Stop(SoundId id)
{
	m_pRealSoundSystem->Stop(id);

	std::stringstream messageStream;
	messageStream << "Stopping: " << id << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::Resume(dae::SoundId id)
{
	m_pRealSoundSystem->Resume(id);

	std::stringstream messageStream;
	messageStream << "Resuming: " << id << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}

void dae::LoggingSoundSystem::RegisterSound(SoundId id, const std::string& path)
{
	m_pRealSoundSystem->RegisterSound(id, path);

	std::stringstream messageStream;
	messageStream << "Registered: " << id << " with path: " << path << "\n";
	std::string message = messageStream.str();
	std::cout << message;
}
