#pragma once
#include "NullSoundSystem.h"
#include "NullInputManager.h"
#include "NullSceneManager.h"
#include <memory>

namespace dae
{
	class SoundSystemBase;
	class InputManagerBase;
	class SceneManagerBase;

	class ServiceLocator final
	{
	public: // TODO: SERVICELOCATOR: add renderer, resource manager
		static void Destroy();

		static SoundSystemBase& GetSoundSystem();
		static void RegisterSoundSystem(SoundSystemBase* soundSystem);

		static InputManagerBase& GetInputManager();
		static void RegisterInputManager(InputManagerBase* inputManager);

		static SceneManagerBase& GetSceneManager();
		static void RegisterSceneManager(SceneManagerBase* sceneManager);

	private:
		template <typename T>
		static T* SetService(T* service, T* newService, T& defaultService)
		{
			if (newService == nullptr)
			{
				//service = &defaultService;
				return &defaultService;
			}

			if (service != &defaultService)
			{
				delete service;
			}
			//service = newService;
			return newService;
		}
		
		static SoundSystemBase* m_pSoundSystemInst;
		static NullSoundSystem m_DefaultSoundSystem;

		static InputManagerBase* m_pInputManagerInst;
		static NullInputManager m_DefaultInputManager;

		static SceneManagerBase* m_pSceneManagerInst;
		static NullSceneManager m_DefaultSceneManager;
	};
}