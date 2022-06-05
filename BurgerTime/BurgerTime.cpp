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

#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "MoveUpCommand.h"
#include "MoveDownCommand.h"
#include "MoveComponent.h"

#include "LevelPrefab.h"

void CreatePlayer(dae::Scene* scene, float healtPosX, float peterPosX, int playerId)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Peter
	auto peterObject = std::make_shared<dae::GameObject>(glm::vec3(peterPosX, 480.f / 2.f, 0.f));
	peterObject->AddComponent(std::make_shared<PeterPepperComponent>(peterObject));
	auto peterHealthComp = peterObject->AddComponent(std::make_shared<HealthComponent>(peterObject, 3));
	peterObject->AddComponent(std::make_shared<dae::Texture2DComponent>(peterObject, "PP_Idle.png"));
	peterObject->AddComponent(std::make_shared<MoveComponent>(peterObject, 20.f));

	// Input
	auto& input = dae::ServiceLocator::GetInputManager();
	//auto damageCommand = std::make_shared<TakeDamageCommand>(peterObject);
	input.AddInput(dae::InputAction(playerId, dae::ButtonState::pressed, std::make_shared<MoveLeftCommand>(peterObject), dae::ControllerButton::DPadLeft, dae::KeyboardKey::K_A));
	input.AddInput(dae::InputAction(playerId, dae::ButtonState::pressed, std::make_shared<MoveRightCommand>(peterObject), dae::ControllerButton::DPadRight, dae::KeyboardKey::K_D));
	input.AddInput(dae::InputAction(playerId, dae::ButtonState::pressed, std::make_shared<MoveUpCommand>(peterObject), dae::ControllerButton::DPadUp, dae::KeyboardKey::K_W));
	input.AddInput(dae::InputAction(playerId, dae::ButtonState::pressed, std::make_shared<MoveDownCommand>(peterObject), dae::ControllerButton::DPadDown, dae::KeyboardKey::K_S));
	input.AddInput(dae::InputAction(playerId, dae::ButtonState::pressed, std::make_shared<MoveDownCommand>(peterObject), dae::ControllerButton::ButtonA, dae::KeyboardKey::K_SPACE));

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

	LoadLevel(scene);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto soAssignment = std::make_shared<dae::GameObject>(80, 20, 0);
	auto textAssignment = std::make_shared<dae::TextComponent>(soAssignment, "Programming 4 Assignment", font, glm::vec3(255.f, 255.f, 255.f));
	soAssignment->AddComponent(textAssignment);
	scene->Add(soAssignment);

	CreatePlayer(scene, 0.f, 640.f / 3.f, 0);
	CreatePlayer(scene, 450.f, 640.f / 3.f * 2.f, 1);

	dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);
}

int main(int, char* []) 
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}