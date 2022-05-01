#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "SoundSystemBase.h"
#include "InputManagerBase.h"
#include "SceneManagerBase.h"


using namespace dae;

NullSoundSystem ServiceLocator::m_DefaultSoundSystem;
SoundSystemBase* ServiceLocator::m_pSoundSystemInst = &m_DefaultSoundSystem;

NullInputManager ServiceLocator::m_DefaultInputManager;
InputManagerBase* ServiceLocator::m_pInputManagerInst = &m_DefaultInputManager;

NullSceneManager ServiceLocator::m_DefaultSceneManager;
SceneManagerBase* ServiceLocator::m_pSceneManagerInst = &m_DefaultSceneManager;

void ServiceLocator::Destroy()
{
	if (m_pSoundSystemInst != &m_DefaultSoundSystem)
	{
		delete m_pSoundSystemInst;
		m_pSoundSystemInst = &m_DefaultSoundSystem;
	}

	if (m_pInputManagerInst != &m_DefaultInputManager)
	{
		delete m_pInputManagerInst;
		m_pInputManagerInst = &m_DefaultInputManager;
	}

	if (m_pSceneManagerInst != &m_DefaultSceneManager)
	{
		delete m_pSceneManagerInst;
		m_pSceneManagerInst = &m_DefaultSceneManager;
	}
}


SoundSystemBase& ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystemInst;
}

void ServiceLocator::RegisterSoundSystem(SoundSystemBase* soundSystem)
{
	m_pSoundSystemInst = SetService<SoundSystemBase>(m_pSoundSystemInst, soundSystem, m_DefaultSoundSystem);
}

InputManagerBase& ServiceLocator::GetInputManager()
{
	return *m_pInputManagerInst;
}

void ServiceLocator::RegisterInputManager(InputManagerBase* inputManager)
{
	m_pInputManagerInst = SetService<InputManagerBase>(m_pInputManagerInst, inputManager, m_DefaultInputManager);
}

SceneManagerBase& dae::ServiceLocator::GetSceneManager()
{
	return *m_pSceneManagerInst;
}

void dae::ServiceLocator::RegisterSceneManager(SceneManagerBase* sceneManager)
{
	m_pSceneManagerInst = SetService<SceneManagerBase>(m_pSceneManagerInst, sceneManager, m_DefaultSceneManager);
}
