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
#include "LevelPathfindingComponent.h"
#include "MovementControllerComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "CollisionHandlerComponent.h"

#include "BulletPoolComponent.h"
#include "GunComponent.h"
#include "FireCommand.h"

void LoadGame()
{
	std::string sceneName{ "Level0" };
	auto scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);
	auto& inputM = dae::ServiceLocator::GetInputManager();
	
	//dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	//dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);

	// LEVEL
	auto level_go = std::make_shared<dae::GameObject>(100.f, 20.f, 0.f);
	std::shared_ptr<LevelLayoutComponent> level_layout = std::make_shared<LevelLayoutComponent>(level_go, "../Data/LevelLayout01.csv", 16, 8);
	level_go->AddComponent(level_layout);
	auto level_visuals = std::make_shared<LevelVisualComponent>(level_go, level_layout, "Level/wall.png", "Level/void.png", "Level/path.png");
	level_go->AddComponent(level_visuals);
	auto level_movement = std::make_shared<LevelMovementComponent>(level_go, level_layout);
	level_go->AddComponent(level_movement);
	auto level_pathfinding = std::make_shared<LevelPathfindingComponent>(level_go, level_layout);
	level_go->AddComponent(level_pathfinding);

	scene->Add(level_go);

	// COLLISION HANDLER
	auto collisionHandler_go = std::make_shared<dae::GameObject>();
	auto collisionHandler = std::make_shared<CollisionHandlerComponent>(collisionHandler_go);
	collisionHandler_go->AddComponent(collisionHandler);
	collisionHandler->AddCollisionIgnore(CollisionHandlerComponent::Layer::Player, CollisionHandlerComponent::Layer::PlayerBullet);
	collisionHandler->AddCollisionIgnore(CollisionHandlerComponent::Layer::EnemyBullet, CollisionHandlerComponent::Layer::PlayerBullet);

	scene->Add(collisionHandler_go);

	// BULLET POOL
	auto bulletPool_go = std::make_shared<dae::GameObject>();
	auto bulletPool_player_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletPlayer.png", BulletComponent::Type::Player, CollisionHandlerComponent::Layer::Player, 20);
	bulletPool_go->AddComponent(bulletPool_player_comp);
	auto bulletPool_enemy_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletNPC.png", BulletComponent::Type::Enemy, CollisionHandlerComponent::Layer::Enemy, 20);
	bulletPool_go->AddComponent(bulletPool_player_comp);

	scene->Add(bulletPool_go);

	// PLAYER
	auto player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[0]);
	auto playerRed_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);

	// VISUALS
	auto tank_go = std::make_shared<dae::GameObject>();
	auto playerRed_texture = std::make_shared<dae::Texture2DComponent>(tank_go, "Sprites/RedTank.png");
	playerRed_texture->SetRenderPositionOffset(glm::vec2(playerRed_texture->GetWidth() / 2.f, playerRed_texture->GetHeight() / 2.f));
	tank_go->AddComponent(playerRed_texture);
	tank_go->SetParent(playerRed_go, tank_go, false);

	// MOVEMENT
	playerRed_go->AddComponent(std::make_shared<MoveComponent>(playerRed_go, tank_go, level_movement, 60.f));

	// GUN
	auto gun_go = std::make_shared<dae::GameObject>();
	gun_go->AddComponent(std::make_shared<GunComponent>(gun_go, bulletPool_player_comp, BulletComponent::Type::Player, 5, 0.2f, 300.f));
	auto gun_texture = std::make_shared<dae::Texture2DComponent>(gun_go, "Sprites/RedTankGun.png");
	gun_texture->SetRenderPositionOffset(glm::vec2(gun_texture->GetWidth() / 2.f, gun_texture->GetHeight() / 2.f));
	gun_go->AddComponent(gun_texture);
	gun_go->SetParent(playerRed_go, gun_go, false);

	// HEALTH
	playerRed_go->AddComponent(std::make_shared<HealthComponent>(playerRed_go, 3, 1.f));
	playerRed_go->AddComponent(std::make_shared<CollisionComponent>(playerRed_go, static_cast<float>(playerRed_texture->GetWidth()), static_cast<float>(playerRed_texture->GetHeight())));

	playerRed_go->GetComponent<CollisionComponent>()->AddObserver(playerRed_go->GetComponent<HealthComponent>());
	collisionHandler->AddCollider(playerRed_go->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Player);

	scene->Add(playerRed_go);

	// INPUT
	auto movementInput = dae::InputAction(0, std::make_shared<MoveCommand>(playerRed_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(0, std::make_shared<FireCommand>(gun_go->GetComponent<GunComponent>(), dae::Joystick::RightStick), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// ENEMY
	const auto& enemy_startPosVec = level_layout->GetEnemyStartPositions();
	for (size_t i = 0; i < 1; ++i)
	{
		const auto& enemy_startPos = level_layout->GetGridCenter(enemy_startPosVec[i]);
		auto enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);
		auto enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, "Sprites/BlueTank.png");
		enemy_texture->SetRenderPositionOffset(glm::vec2(enemy_texture->GetWidth() / 2.f, enemy_texture->GetHeight() / 2.f));
		enemy_go->AddComponent(enemy_texture);
		auto enemy_moveComp = std::make_shared<MoveComponent>(enemy_go, enemy_go, level_movement, 40.f);
		enemy_go->AddComponent(enemy_moveComp);
		enemy_go->AddComponent(std::make_shared<MovementControllerComponent>(enemy_go, playerRed_go, enemy_moveComp, level_pathfinding, level_layout));
		
		enemy_go->AddComponent(std::make_shared<HealthComponent>(enemy_go, 3, 1.f));
		enemy_go->AddComponent(std::make_shared<CollisionComponent>(enemy_go, static_cast<float>(enemy_texture->GetWidth()), static_cast<float>(enemy_texture->GetHeight())));

		enemy_go->GetComponent<CollisionComponent>()->AddObserver(enemy_go->GetComponent<HealthComponent>());
		collisionHandler->AddCollider(enemy_go->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Enemy);

		scene->Add(enemy_go);
	}
}

int main(int, char* [])
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}