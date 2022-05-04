#include "BurgerTimePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "TextComponent.h"
#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "PeterPepperComponent.h"
#include "Texture2DComponent.h"

#include "TakeDamageCommand.h"
#include "ServiceLocator.h"
#include <functional>
#include "SDLSoundSystem.h"
#include "BaseSoundSystem.h"
#include "BaseInputManager.h"
#include "BaseSceneManager.h"

void CreatePlayer(dae::Scene* scene, float healtPosX, float peterPosX, int)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Peter
	auto peterObject = std::make_shared<dae::GameObject>(glm::vec3(peterPosX, 480.f / 2.f, 0.f));
	peterObject->AddComponent(std::make_shared<PeterPepperComponent>(peterObject));
	auto peterHealthComp = peterObject->AddComponent(std::make_shared<HealthComponent>(peterObject, 3));
	peterObject->AddComponent(std::make_shared<dae::Texture2DComponent>(peterObject, "PP_Idle.png"));

	// Input
	//auto& input = dae::ServiceLocator::GetInputManager();
	//auto damageCommand = std::make_shared<TakeDamageCommand>(peterObject);
	//input.AddInput(dae::InputAction(playerId, dae::ButtonState::downThisFrame, damageCommand, dae::ControllerButton::ButtonA));

	// Health
	auto healthObject = std::make_shared<dae::GameObject>(glm::vec3(healtPosX, 70.f, 0.f));
	auto healthTextComp = std::make_shared<dae::TextComponent>(healthObject, "0", font, glm::vec3(255.f, 255.f, 255.f));
	healthObject->AddComponent(healthTextComp);
	auto healthDisplayComp = std::make_shared<HealthDisplayComponent>(healthObject, healthTextComp, peterHealthComp);
	healthObject->AddComponent(healthDisplayComp);

	peterHealthComp->AddObserver(healthDisplayComp);

	scene->Add(healthObject);
	scene->Add(peterObject);
}

void LoadGame()
{
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene("Demo");

	//auto soBg = std::make_shared<GameObject>();
	//auto textureBg = std::make_shared<Texture2DComponent>(soBg, "background.jpg");
	//soBg->AddComponent(textureBg);
	//scene.Add(soBg);

	//auto soLogo = std::make_shared<GameObject>(216.f, 180.f, 0.f);
	//auto textureLogo = std::make_shared<Texture2DComponent>(soLogo, "logo.png");
	//soLogo->AddComponent(textureLogo);
	//scene.Add(soLogo);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto soAssignment = std::make_shared<dae::GameObject>(80, 20, 0);
	auto textAssignment = std::make_shared<dae::TextComponent>(soAssignment, "Programming 4 Assignment", font, glm::vec3(255.f, 255.f, 255.f));
	soAssignment->AddComponent(textAssignment);
	scene->Add(soAssignment);

	//auto soFPS = std::make_shared<GameObject>();
	//auto textFPS = std::make_shared<dae::TextComponent>(soFPS, "0", font, glm::vec3(255.f, 255.f, 255.f));
	//soFPS->AddComponent(textFPS);
	//auto FPS = std::make_shared<dae::FPSComponent>(soFPS, textFPS);
	//soFPS->AddComponent(FPS);
	//scene.Add(soFPS);

	CreatePlayer(scene, 0.f, 640.f / 3.f, 0);
	CreatePlayer(scene, 450.f, 640.f / 3.f * 2.f, 1);

	dae::ServiceLocator::GetSoundSystem().RegisterSound(0, "../Data/Yay.wav");
	dae::ServiceLocator::GetSoundSystem().Play(0, 1.f);
}

int main(int, char* []) 
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}