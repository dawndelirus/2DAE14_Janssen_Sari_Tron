#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "BaseSoundSystem.h"
#include "BaseInputManager.h"
#include "BaseSceneManager.h"

using namespace dae;

NullSoundSystem ServiceLocator::m_DefaultSoundSystem;
BaseSoundSystem* ServiceLocator::m_pSoundSystemInst = &m_DefaultSoundSystem;

NullInputManager ServiceLocator::m_DefaultInputManager;
BaseInputManager* ServiceLocator::m_pInputManagerInst = &m_DefaultInputManager;

NullSceneManager ServiceLocator::m_DefaultSceneManager;
BaseSceneManager* ServiceLocator::m_pSceneManagerInst = &m_DefaultSceneManager;

BaseSoundSystem& ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystemInst;
}

void ServiceLocator::RegisterSoundSystem(BaseSoundSystem* soundSystem)
{
	m_pSoundSystemInst = SetService<BaseSoundSystem>(soundSystem, m_DefaultSoundSystem);
}

BaseInputManager& ServiceLocator::GetInputManager()
{
	return *m_pInputManagerInst;
}

void ServiceLocator::RegisterInputManager(BaseInputManager* inputManager)
{
	m_pInputManagerInst = SetService<BaseInputManager>(inputManager, m_DefaultInputManager);
}

BaseSceneManager& dae::ServiceLocator::GetSceneManager()
{
	return *m_pSceneManagerInst;
}

void dae::ServiceLocator::RegisterSceneManager(BaseSceneManager* sceneManager)
{
	m_pSceneManagerInst = SetService<BaseSceneManager>(sceneManager, m_DefaultSceneManager);
}
