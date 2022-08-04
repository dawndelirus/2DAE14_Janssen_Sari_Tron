#pragma once
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <functional>
#include <memory>

#include "Minigin.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "BaseSceneManager.h"

#include "MoveComponent.h"
#include "MoveCommand.h"

#include "LevelLayoutComponent.h"
#include "LevelVisualComponent.h"
#include "LevelMovementComponent.h"

void LoadGame()
{
	auto scene = dae::ServiceLocator::GetSceneManager().CreateScene("Level0");
	auto& inputM = dae::ServiceLocator::GetInputManager();
	
	//dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	//dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);

	// LEVEL
	auto level_go = std::make_shared<dae::GameObject>(100.f, 20.f, 0.f);
	std::shared_ptr<LevelLayoutComponent> level_layout = std::make_shared<LevelLayoutComponent>(level_go, "../Data/LevelLayout0.csv", 16, 16);
	level_go->AddComponent(level_layout);
	auto level_visuals = std::make_shared<LevelVisualComponent>(level_go, level_layout);
	level_go->AddComponent(level_visuals);
	auto level_movement = std::make_shared<LevelMovementComponent>(level_go, level_layout);
	level_go->AddComponent(level_movement);
	
	scene->Add(level_go);

	// PLAYER
	auto player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[0]);
	auto playerRed_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);
	auto playerRed_texture = std::make_shared<dae::Texture2DComponent>(playerRed_go, "Sprites/RedTank.png");
	playerRed_texture->SetRenderPositionOffset(glm::vec2(16.f, 16.f));
	playerRed_go->AddComponent(playerRed_texture);
	playerRed_go->AddComponent(std::make_shared<MoveComponent>(playerRed_go, level_movement, 100.f));

	scene->Add(playerRed_go);

	// INPUT
	auto movementInput = dae::InputAction(0, std::make_shared<MoveCommand>(playerRed_go, playerRed_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);

}

int main(int, char* [])
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}