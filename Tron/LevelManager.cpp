#include "LevelManager.h"
#include "GameInfo.h"

void LevelManager::Initialize()
{
	// create main menu
	m_CurrentLevel = 2;
	LoadLevel("Level");
}

void LevelManager::LoadLevel(const std::string& sceneName)
{
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	//dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);

	// LEVEL
	auto level_go = std::make_shared<dae::GameObject>(100.f, 20.f, 0.f);
	std::shared_ptr<LevelLayoutComponent> level_layout = std::make_shared<LevelLayoutComponent>(level_go, m_LevelPahts[m_CurrentLevel], 16, 8);
	auto level_visuals = std::make_shared<LevelVisualComponent>(level_go, level_layout, "Level/wall.png", "Level/void.png", "Level/path.png");
	auto level_movement = std::make_shared<LevelMovementComponent>(level_go, level_layout);
	auto level_pathfinding = std::make_shared<LevelPathfindingComponent>(level_go, level_layout);

	level_go->AddComponent(level_layout);
	level_go->AddComponent(level_visuals);
	level_go->AddComponent(level_movement);
	level_go->AddComponent(level_pathfinding);

	scene->Add(level_go);

	// COLLISION HANDLER
	auto collisionHandler_go = std::make_shared<dae::GameObject>();
	auto collisionHandler = std::make_shared<CollisionHandlerComponent>(collisionHandler_go);
	collisionHandler_go->AddComponent(collisionHandler);
	collisionHandler->AddCollisionIgnore(CollisionHandlerComponent::Layer::Player, CollisionHandlerComponent::Layer::PlayerBullet);
	collisionHandler->AddCollisionIgnore(CollisionHandlerComponent::Layer::EnemyBullet, CollisionHandlerComponent::Layer::PlayerBullet);
	collisionHandler->AddCollisionIgnore(CollisionHandlerComponent::Layer::EnemyBullet, CollisionHandlerComponent::Layer::Enemy);

	scene->Add(collisionHandler_go);

	// BULLET POOL
	auto bulletPool_go = std::make_shared<dae::GameObject>();
	auto bulletPool_player_go = std::make_shared<dae::GameObject>();
	auto bulletPool_enemy_go = std::make_shared<dae::GameObject>();

	auto bulletPool_player_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletPlayer.png", BulletComponent::Type::Player, CollisionHandlerComponent::Layer::PlayerBullet, 50);
	auto bulletPool_enemy_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletNPC.png", BulletComponent::Type::Enemy, CollisionHandlerComponent::Layer::EnemyBullet, 20);

	bulletPool_player_go->AddComponent(bulletPool_player_comp);
	bulletPool_enemy_go->AddComponent(bulletPool_enemy_comp);

	bulletPool_player_go->SetParent(bulletPool_go, bulletPool_player_go, true);
	bulletPool_enemy_go->SetParent(bulletPool_go, bulletPool_enemy_go, true);

	scene->Add(bulletPool_go);

	// HUD
	auto hud_go = std::make_shared<dae::GameObject>();

	// PLAYER
	std::vector<std::shared_ptr<dae::GameObject>> players{};

	switch (m_CurrentGamemode)
	{
	case LevelManager::GameMode::Coop:
	{
		auto player_go = LoadPlayer(scene, level_go, hud_go, bulletPool_player_comp, collisionHandler, "Sprites/GreenTank.png", "Sprites/GreenTankGun.png", 0);
		players.emplace_back(player_go);
	}
	case LevelManager::GameMode::Single:
	{
		auto player_go = LoadPlayer(scene, level_go, hud_go, bulletPool_player_comp, collisionHandler, "Sprites/RedTank.png", "Sprites/RedTankGun.png", 0);
		players.emplace_back(player_go);
		break;
	}
	case LevelManager::GameMode::Versus:
		break;
	default:
		break;
	}

	// SCORE
	auto score_go = std::make_shared<dae::GameObject>(5.f, 30.f, 0.f);
	auto score_comp = std::make_shared<ScoreComponent>(score_go);
	std::string startText = "Score: " + std::to_string(score_comp->GetScore());
	auto scoreDisplay_comp = std::make_shared<ScoreDisplayComponent>(score_go);

	score_go->AddComponent(std::make_shared<dae::TextComponent>(score_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));
	score_go->AddComponent(scoreDisplay_comp);
	score_go->AddComponent(score_comp);

	score_comp->AddObserver(scoreDisplay_comp);
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());

	score_go->SetParent(hud_go, score_go, true);

	scene->Add(hud_go);

	// ENEMIES
	LoadEnemies(scene, level_go, players, bulletPool_enemy_comp, collisionHandler, score_comp);
}

void LevelManager::LevelClear()
{
	// clear input
	// delete level

	++m_CurrentLevel;
	m_CurrentLevel %= m_LevelPahts.size();

	// load level
}

void LevelManager::LevelFail()
{
	// clear input
	// delete level
	// delete highscore scene if available
	// create highscore scene
}

void LevelManager::AddLevelPath(const std::string& levelPath)
{
	m_LevelPahts.emplace_back(levelPath);
}

std::shared_ptr<dae::GameObject> LevelManager::LoadPlayer(dae::Scene* scene, std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler, const std::string& playerPath, const std::string& gunPath, int playerIdx)
{
	dae::BaseInputManager& inputM = dae::ServiceLocator::GetInputManager();

	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// PLAYER
	const glm::vec2& player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[playerIdx]);
	std::shared_ptr<dae::GameObject> player_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);
	std::shared_ptr<PlayerComponent> player_comp = std::make_shared<PlayerComponent>(player_go);
	player_go->AddComponent(player_comp);

	// VISUALS
	std::shared_ptr<dae::GameObject> visuals_go = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2DComponent> player_texture = std::make_shared<dae::Texture2DComponent>(visuals_go, playerPath);
	player_texture->SetRenderPositionOffset(glm::vec2(player_texture->GetWidth() / 2.f, player_texture->GetHeight() / 2.f));

	visuals_go->AddComponent(player_texture);
	visuals_go->SetParent(player_go, visuals_go, false);

	// MOVEMENT
	player_go->AddComponent(std::make_shared<MoveComponent>(player_go, visuals_go, level_movement, 60.f));

	// GUN + VISUALS
	std::shared_ptr<dae::GameObject> gun_go = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2DComponent> gun_texture = std::make_shared<dae::Texture2DComponent>(gun_go, gunPath);
	gun_texture->SetRenderPositionOffset(glm::vec2(gun_texture->GetWidth() / 2.f, gun_texture->GetHeight() / 2.f));

	gun_go->AddComponent(std::make_shared<GunComponent>(gun_go, bulletPool, BulletComponent::Type::Player, 5, 0.5f, 200.f));
	gun_go->AddComponent(gun_texture);
	gun_go->SetParent(player_go, gun_go, false);

	// HEALTH
	player_go->AddComponent(std::make_shared<HealthComponent>(player_go, GameInfo::GetInstance().GetPlayerHealth(playerIdx), 1.f));
	player_go->AddComponent(std::make_shared<CollisionComponent>(player_go, static_cast<float>(player_texture->GetWidth()), static_cast<float>(player_texture->GetHeight())));

	collisionHandler->AddCollider(player_go->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Player);

	player_go->GetComponent<CollisionComponent>()->AddObserver(player_comp);
	player_comp->AddObserver(player_go->GetComponent<HealthComponent>());

	scene->Add(player_go);

	// INPUT
	auto movementInput = dae::InputAction(playerIdx, std::make_shared<MoveCommand>(player_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(playerIdx, std::make_shared<FireCommand>(gun_go->GetComponent<GunComponent>(), dae::Joystick::RightStick), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// HEALTH DISPLAY
	auto healthDisplay_go = std::make_shared<dae::GameObject>(5.f, 10.f, 0.f);
	auto healthDisplay_comp = std::make_shared<HealthDisplayComponent>(healthDisplay_go);
	std::string startText = "Lives: " + std::to_string(player_go->GetComponent<HealthComponent>()->GetCurrentHealth());

	healthDisplay_go->AddComponent(healthDisplay_comp);
	healthDisplay_go->AddComponent(std::make_shared<dae::TextComponent>(healthDisplay_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));

	player_go->GetComponent<HealthComponent>()->AddObserver(healthDisplay_comp);
	healthDisplay_comp->AddObserver(healthDisplay_go->GetComponent<dae::TextComponent>());

	healthDisplay_go->SetParent(hud_go, healthDisplay_go, true);

	return player_go;
}

void LevelManager::LoadEnemies(dae::Scene* scene, std::shared_ptr<dae::GameObject> level_go, std::vector<std::shared_ptr<dae::GameObject>> player_go, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler, std::shared_ptr<ScoreComponent> score_comp)
{
	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();
	std::shared_ptr<LevelPathfindingComponent> level_pathfinding = level_go->GetComponent<LevelPathfindingComponent>();

	// ENEMY
	auto enemyController_go = std::make_shared<dae::GameObject>();
	auto enemyController = std::make_shared<EnemyControllerComponent>(enemyController_go, collisionHandler);
	enemyController_go->AddComponent(enemyController);

	scene->Add(enemyController_go);

	// BLUE TANKS
	const auto& enemyTank_startPosVec = level_layout->GetEnemyTankStartPositions();
	for (size_t i = 0; i < enemyTank_startPosVec.size(); ++i)
	{
		const glm::vec2& enemy_startPos = level_layout->GetGridCenter(enemyTank_startPosVec[i]);
		std::shared_ptr<dae::GameObject> enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);

		std::shared_ptr<dae::Texture2DComponent> enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, "Sprites/BlueTank.png");
		float textureWidth = static_cast<float>(enemy_texture->GetWidth());
		float textureHeight = static_cast<float>(enemy_texture->GetHeight());
		enemy_texture->SetRenderPositionOffset(glm::vec2(textureWidth / 2.f, textureHeight / 2.f));

		std::shared_ptr<MoveComponent> enemy_moveComponent = std::make_shared<MoveComponent>(enemy_go, enemy_go, level_movement, 30.f);
		std::shared_ptr<MovementControllerComponent> enemy_moveController = std::make_shared<MovementControllerComponent>(enemy_go, player_go, enemy_moveComponent, level_pathfinding, level_layout);

		std::shared_ptr<GunComponent> gunComp = std::make_shared<GunComponent>(enemy_go, bulletPool, BulletComponent::Type::Enemy, 0, 1.f, 200.f);
		std::shared_ptr<EnemyTankComponent> enemyTankComp = std::make_shared<EnemyTankComponent>(enemy_go, enemy_moveController, gunComp, level_layout, 100);

		std::shared_ptr<HealthComponent> enemy_healthComponent = std::make_shared<HealthComponent>(enemy_go, 3, 0.5f);
		std::shared_ptr<CollisionComponent> enemy_collider = std::make_shared<CollisionComponent>(enemy_go, textureWidth, textureHeight);


		enemy_go->AddComponent(gunComp);
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

	// RECOGNIZERS
	const auto& enemyRecognizer_startPosVec = level_layout->GetEnemyRecognizerStartPositions();
	for (size_t i = 0; i < enemyRecognizer_startPosVec.size(); ++i)
	{
		const glm::vec2& enemy_startPos = level_layout->GetGridCenter(enemyRecognizer_startPosVec[i]);
		std::shared_ptr<dae::GameObject> enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);
		auto enemyRecComp = std::make_shared<EnemyRecognizerComponent>(enemy_go, 250);

		std::shared_ptr<dae::Texture2DComponent> enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, "Sprites/Recognizer.png");
		float textureWidth = static_cast<float>(enemy_texture->GetWidth());
		float textureHeight = static_cast<float>(enemy_texture->GetHeight());
		enemy_texture->SetRenderPositionOffset(glm::vec2(textureWidth / 2.f, textureHeight / 2.f));

		std::shared_ptr<MoveComponent> enemy_moveComponent = std::make_shared<MoveComponent>(enemy_go, enemy_go, level_movement, 60.f);
		std::shared_ptr<MovementControllerComponent> enemy_moveController = std::make_shared<MovementControllerComponent>(enemy_go, player_go, enemy_moveComponent, level_pathfinding, level_layout);

		std::shared_ptr<HealthComponent> enemy_healthComponent = std::make_shared<HealthComponent>(enemy_go, 3, 0.5f);
		std::shared_ptr<CollisionComponent> enemy_collider = std::make_shared<CollisionComponent>(enemy_go, textureWidth, textureHeight);

		enemy_go->AddComponent(enemy_texture);
		enemy_go->AddComponent(enemy_moveComponent);
		enemy_go->AddComponent(enemy_moveController);
		enemy_go->AddComponent(enemy_healthComponent);
		enemy_go->AddComponent(enemy_collider);
		enemy_go->AddComponent(enemyRecComp);

		enemy_collider->AddObserver(enemyRecComp);
		enemyRecComp->AddObserver(enemy_healthComponent);
		collisionHandler->AddCollider(enemy_collider, CollisionHandlerComponent::Layer::Enemy);

		enemy_healthComponent->AddObserver(enemyRecComp);
		enemyRecComp->AddObserver(score_comp);

		enemyController->AddEnemies(enemy_go);
		enemy_healthComponent->AddObserver(enemyController);

		scene->Add(enemy_go);
	}
}

