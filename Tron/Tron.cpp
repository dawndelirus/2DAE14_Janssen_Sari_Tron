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

#include "EnemyControllerComponent.h"
#include "EnemyTankComponent.h"

#include "HealthDisplayComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"

void LoadGame()
{
	std::string sceneName{ "Level0" };
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);
	dae::BaseInputManager& inputM = dae::ServiceLocator::GetInputManager();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

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
	auto bulletPool_player_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletPlayer.png", BulletComponent::Type::Player, CollisionHandlerComponent::Layer::PlayerBullet, 20);
	bulletPool_go->AddComponent(bulletPool_player_comp);
	auto bulletPool_enemy_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletNPC.png", BulletComponent::Type::Enemy, CollisionHandlerComponent::Layer::EnemyBullet, 20);
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

	// HUD
	auto hud_go = std::make_shared<dae::GameObject>();

	// HEALTH
	auto healthDisplay_go = std::make_shared<dae::GameObject>(10.f, 10.f, 0.f);
	auto healthDisplay_comp = std::make_shared<HealthDisplayComponent>(healthDisplay_go);
	healthDisplay_go->AddComponent(healthDisplay_comp);
	std::string startText = "Lives: " + std::to_string(playerRed_go->GetComponent<HealthComponent>()->GetCurrentHealth());
	healthDisplay_go->AddComponent(std::make_shared<dae::TextComponent>(healthDisplay_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));

	playerRed_go->GetComponent<HealthComponent>()->AddObserver(healthDisplay_comp);
	healthDisplay_comp->AddObserver(healthDisplay_go->GetComponent<dae::TextComponent>());

	healthDisplay_go->SetParent(hud_go, healthDisplay_go, true);

	// SCORE
	auto score_go = std::make_shared<dae::GameObject>(10.f, 30.f, 0.f);
	auto score_comp = std::make_shared<ScoreComponent>(score_go);
	startText = "Score: " + std::to_string(score_comp->GetScore());
	score_go->AddComponent(std::make_shared<dae::TextComponent>(score_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));
	auto scoreDisplay_comp = std::make_shared<ScoreDisplayComponent>(score_go);
	score_go->AddComponent(scoreDisplay_comp);
	score_go->AddComponent(score_comp);

	score_comp->AddObserver(scoreDisplay_comp);
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());

	score_go->SetParent(hud_go, score_go, true);
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());


	scene->Add(hud_go);

	// INPUT
	auto movementInput = dae::InputAction(0, std::make_shared<MoveCommand>(playerRed_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(0, std::make_shared<FireCommand>(gun_go->GetComponent<GunComponent>(), dae::Joystick::RightStick), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// ENEMY
	auto enemyController_go = std::make_shared<dae::GameObject>();
	auto enemyController = std::make_shared<EnemyControllerComponent>(enemyController_go, collisionHandler);
	enemyController_go->AddComponent(enemyController);

	scene->Add(enemyController_go);

	const auto& enemy_startPosVec = level_layout->GetEnemyTankStartPositions();
	for (size_t i = 0; i < enemy_startPosVec.size(); ++i)
	//for (size_t i = 0; i < 1; ++i)
	{
		const glm::vec2& enemy_startPos = level_layout->GetGridCenter(enemy_startPosVec[i]);
		std::shared_ptr<dae::GameObject> enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);
		auto enemyTankComp = std::make_shared<EnemyTankComponent>(enemy_go, 100);

		std::shared_ptr<dae::Texture2DComponent> enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, "Sprites/BlueTank.png");
		float textureWidth = static_cast<float>(enemy_texture->GetWidth());
		float textureHeight = static_cast<float>(enemy_texture->GetHeight());
		enemy_texture->SetRenderPositionOffset(glm::vec2(textureWidth / 2.f, textureHeight / 2.f));

		std::shared_ptr<MoveComponent> enemy_moveComponent = std::make_shared<MoveComponent>(enemy_go, enemy_go, level_movement, 40.f);
		std::shared_ptr<MovementControllerComponent> enemy_moveController = std::make_shared<MovementControllerComponent>(enemy_go, playerRed_go, enemy_moveComponent, level_pathfinding, level_layout);

		std::shared_ptr<HealthComponent> enemy_healthComponent = std::make_shared<HealthComponent>(enemy_go, 3, 1.f);
		std::shared_ptr<CollisionComponent> enemy_collider = std::make_shared<CollisionComponent>(enemy_go, textureWidth, textureHeight);
		
		enemy_go->AddComponent(enemy_texture);
		enemy_go->AddComponent(enemy_moveComponent);
		enemy_go->AddComponent(enemy_moveController);
		enemy_go->AddComponent(enemy_healthComponent);
		enemy_go->AddComponent(enemy_collider);
		enemy_go->AddComponent(enemyTankComp);

		enemy_collider->AddObserver(enemyTankComp);
		enemyTankComp->AddObserver(enemy_healthComponent);
		collisionHandler->AddCollider(enemy_collider, CollisionHandlerComponent::Layer::Enemy);

		enemy_healthComponent->AddObserver(enemyTankComp);
		enemyTankComp->AddObserver(score_comp);

		enemyController->AddEnemies(enemy_go);
		enemy_healthComponent->AddObserver(enemyController);

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