#pragma once
#include "Singleton.h"
#include <vector>
#include <string>
#include <memory>
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
#include "EnemyRecognizerComponent.h"

#include "HealthDisplayComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"
#include "PlayerComponent.h"

class LevelManager final : public dae::Singleton<LevelManager>
{
public:
	enum class GameMode
	{
		Single,
		Coop,
		Versus
	};
	
	void Initialize();

	void LevelClear();
	void LevelFail();

	void AddLevelPath(const std::string& levelPath);

private:
	enum class GameState
	{
		MainMenu,
		Game,
		PauseMenu,
		HighScore
	};

	void LoadLevel();

	std::shared_ptr<dae::GameObject> LoadPlayer(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go
		, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler
		, const std::string& playerPath, const std::string& gunPath, int playerIdx);

	std::shared_ptr<dae::GameObject> LoadPlayerVersus(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go
		, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler
		, const std::string& playerPath, const std::string& gunPath, int playerIdx);

	std::shared_ptr<EnemyControllerComponent> LoadEnemies(dae::Scene* scene, std::shared_ptr<dae::GameObject> level_go, std::vector<std::shared_ptr<dae::GameObject>> player_go
		, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler
		, std::shared_ptr<ScoreComponent> score_comp);

	std::shared_ptr<dae::GameObject> CreateEnemy(std::shared_ptr<dae::GameObject> level_go, std::vector<std::shared_ptr<dae::GameObject>> targets, const std::string& image, int startIdx, float movementSpeed);

	GameMode m_CurrentGamemode{};
	GameState m_CurrentGamestate{};
	size_t m_CurrentLevel{};
	std::vector<std::string> m_LevelPahts{};
};

