#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "Scene.h"
#include "BaseInputManager.h"
#include "CollisionHandlerComponent.h"

#include <vector>
#include <string>
#include <memory>
#include <map>

class BulletPoolComponent;
class EnemyControllerComponent;
class ScoreComponent;

class LevelManager final : public dae::Singleton<LevelManager>
{
public:
	enum class GameMode
	{
		Single,
		Coop,
		Versus
	};

	enum class InputStates
	{
		Start,
		Pause,
		Defeat,
		Resume
	};

	void PlayerDied(int idx);

	void Initialize();

	void HandleInput(InputStates input);

	void StartGame();
	void LevelClear();
	void LevelFail();

	void NamesEntered();

	void AddLevelPath(const std::string& levelPath);

	GameMode CycleGameMode();
	GameMode GetGameMode() const;

private:
	enum class GameState
	{
		MainMenu,
		Game,
		PauseMenu,
		HighScore
	};

	void CreateMainMenu();
	void CreatePauseMenu();
	void CreatePlayerInputJoystickKeyboard(std::shared_ptr<dae::GameObject> player_go, int playerIdx);
	void CreatePlayerInputKeyboard(std::shared_ptr<dae::GameObject> player_go, int playerIdx);
	void CreatePlayerInputJoystick(std::shared_ptr<dae::GameObject> player_go, int playerIdx);

	std::shared_ptr<dae::GameObject> CreateEnterName(const std::string& sceneName);
	void CreateDisplayHighscore(const std::string& sceneName);

	void LoadLevel();

	std::shared_ptr<dae::GameObject> LoadPlayer(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go
		, std::shared_ptr<CollisionHandlerComponent> collisionHandler, CollisionHandlerComponent::Layer layer, const std::string& playerPath
		, int playerIdx, float movementSpeed);

	std::shared_ptr<EnemyControllerComponent> LoadEnemies(dae::Scene* scene, std::shared_ptr<dae::GameObject> level_go
		, std::vector<std::shared_ptr<dae::GameObject>> player_go, std::shared_ptr<BulletPoolComponent> bulletPool
		, std::shared_ptr<CollisionHandlerComponent> collisionHandler, std::shared_ptr<ScoreComponent> score_comp);

	std::shared_ptr<dae::GameObject> CreateEnemy(std::shared_ptr<dae::GameObject> level_go
		, std::shared_ptr<EnemyControllerComponent> controller, const std::string& image, int startIdx, float movementSpeed);

	std::shared_ptr<dae::GameObject> m_HighScore;

	GameMode m_CurrentGamemode{};
	GameState m_CurrentGamestate{GameState::MainMenu};
	size_t m_CurrentLevel{};
	std::vector<std::string> m_LevelPahts{};

	int m_PlayerDiedAmount{};

	std::map<std::string, std::shared_ptr<dae::BaseInputManager>> m_InputManagers{};
};

