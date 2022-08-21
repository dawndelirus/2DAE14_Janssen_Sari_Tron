#include "LevelManager.h"
#include "GameInfo.h"
#include "ServiceLocator.h"

#include "GameObject.h"
#include "Texture2DComponent.h"
#include "BaseSceneManager.h"
#include "TextComponent.h"
#include "ResourceManager.h"

#include "MoveComponent.h"
#include "MoveCommand.h"

#include "BulletPoolComponent.h"
#include "GunComponent.h"
#include "FireCommand.h"

#include "StartGameCommand.h"
#include "QuitGameCommand.h"
#include "SwapGamemodeCommand.h"
#include "NameEnteredCommand.h"

#include "LevelLayoutComponent.h"
#include "LevelVisualComponent.h"
#include "LevelMovementComponent.h"
#include "LevelPathfindingComponent.h"

#include "CollisionComponent.h"
#include "CollisionHandlerComponent.h"

#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"

#include "HealthComponent.h"
#include "HealthDisplayComponent.h"

#include "MovementControllerComponent.h"
#include "EnemyControllerComponent.h"
#include "EnemyTankComponent.h"
#include "EnemyRecognizerComponent.h"

#include "PlayerComponent.h"

#include "HighScoreComponent.h"
#include <sstream>


void LevelManager::PlayerDied(int idx)
{
	switch (m_CurrentGamemode)
	{
	case LevelManager::GameMode::Single:
		HandleInput(LevelManager::InputStates::Defeat);
		break;
	case LevelManager::GameMode::Coop:
		++m_PlayerDiedAmount;
		if (m_PlayerDiedAmount == 2)
		{
			HandleInput(LevelManager::InputStates::Defeat);
			m_PlayerDiedAmount = 0;
		}
		break;
	case LevelManager::GameMode::Versus:
		if (idx == 0)
		{
			HandleInput(LevelManager::InputStates::Defeat);
		}
		break;
	}
}

void LevelManager::Initialize()
{
	CreateMainMenu();
	dae::ServiceLocator::GetSceneManager().SetActiveScene("MainMenu");
}

void LevelManager::HandleInput(InputStates input)
{
	switch (m_CurrentGamestate)
	{
	case LevelManager::GameState::MainMenu:
		if (input == LevelManager::InputStates::Start)
		{
			m_CurrentGamestate = LevelManager::GameState::Game;
			StartGame();
		}
		break;
	case LevelManager::GameState::Game:
		if (input == LevelManager::InputStates::Pause)
		{
			m_CurrentGamestate = LevelManager::GameState::PauseMenu;
		}
		else if (input == LevelManager::InputStates::Defeat)
		{
			m_CurrentGamestate = LevelManager::GameState::HighScore;
			LevelFail();
		}
		break;
	case LevelManager::GameState::PauseMenu:
		if (input == LevelManager::InputStates::Resume)
		{
			m_CurrentGamestate = LevelManager::GameState::Game;
		}
		break;
	case LevelManager::GameState::HighScore:
		if (input == LevelManager::InputStates::Start)
		{
			m_CurrentGamestate = LevelManager::GameState::Game;
			StartGame();
			dae::ServiceLocator::GetSceneManager().RemoveScene("DisplayHighscore");
		}
		break;
	default:
		break;
	}
}

void LevelManager::CreateMainMenu()
{
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene("MainMenu");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& inputM = dae::ServiceLocator::GetInputManager();

	std::string text = "A/SPACE to start";
	auto start_go = std::make_shared<dae::GameObject>(glm::vec3{170.f, 60.f, 0.f});
	start_go->AddComponent(std::make_shared<dae::TextComponent>(start_go, text, font, glm::vec3(0.f, 255.f, 100.f)));

	text = "B/ESC to quit";
	auto quit_go = std::make_shared<dae::GameObject>(glm::vec3{ 200.f, 160.f, 0.f });
	quit_go->AddComponent(std::make_shared<dae::TextComponent>(quit_go, text, font, glm::vec3(255.f, 50.f, 50.f)));

	text = "Y/TAB to change";
	auto cycle_go = std::make_shared<dae::GameObject>(glm::vec3{ 180.f, 260.f, 0.f });
	cycle_go->AddComponent(std::make_shared<dae::TextComponent>(cycle_go, text, font, glm::vec3(255.f, 255.f, 50.f)));

	text = "Gamemode: ";
	auto gamemode_go = std::make_shared<dae::GameObject>(glm::vec3{ 200.f, 320.f, 0.f });
	gamemode_go->AddComponent(std::make_shared<dae::TextComponent>(gamemode_go, text, smallFont, glm::vec3(255.f, 255.f, 255.f)));

	text = "Singleplayer";
	auto mode_go = std::make_shared<dae::GameObject>(glm::vec3{ 330.f, 320.f, 0.f });
	mode_go->AddComponent(std::make_shared<dae::TextComponent>(mode_go, text, smallFont, glm::vec3(255.f, 255.f, 255.f)));

	scene->Add(start_go);
	scene->Add(quit_go);
	scene->Add(cycle_go);
	scene->Add(gamemode_go);
	scene->Add(mode_go);

	auto startInput = dae::InputAction(0, dae::ButtonState::downThisFrame, std::make_shared<StartGameCommand>(nullptr), dae::ControllerButton::ButtonA, dae::KeyboardKey::K_SPACE);
	inputM.AddInput(startInput);
	auto quitInput = dae::InputAction(0, dae::ButtonState::releasedThisFrame, std::make_shared<QuitGameCommand>(nullptr), dae::ControllerButton::ButtonB, dae::KeyboardKey::K_Esc);
	inputM.AddInput(quitInput);
	auto swapInput = dae::InputAction(0, dae::ButtonState::releasedThisFrame, std::make_shared<SwapGamemodeCommand>(mode_go->GetComponent<dae::TextComponent>()), dae::ControllerButton::ButtonY, dae::KeyboardKey::K_Tab);
	inputM.AddInput(swapInput);
}

std::shared_ptr<dae::GameObject> LevelManager::CreateEnterName(const std::string& sceneName)
{
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);
	auto bigFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto mediumFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	std::string text = "SCORE: " + std::to_string(GameInfo::GetInstance().GetPlayerScore());
	auto score_go = std::make_shared<dae::GameObject>(glm::vec3{ 200.f, 60.f, 0.f });
	score_go->AddComponent(std::make_shared<dae::TextComponent>(score_go, text, bigFont, glm::vec3(0.f, 255.f, 100.f)));
	
	if (m_CurrentGamemode == LevelManager::GameMode::Single)
	{
		text = "PLAYER: ";
	}
	else
	{
		text = "PLAYER 1: ";
	}

	auto player1_go = std::make_shared<dae::GameObject>(glm::vec3{ 100.f, 160.f, 0.f });
	player1_go->AddComponent(std::make_shared<dae::TextComponent>(player1_go, text, mediumFont, glm::vec3(255.f, 10.f, 10.f)));

	auto player1_text_go = std::make_shared<dae::GameObject>(glm::vec3{ 260.f, 160.f, 0.f });
	player1_text_go->AddComponent(std::make_shared<dae::TextComponent>(player1_text_go, "", mediumFont, glm::vec3(255.f, 255.f, 255.f)));

	std::shared_ptr<dae::TextComponent> player2Text{};
	if (m_CurrentGamemode != LevelManager::GameMode::Single)
	{
		text = "PLAYER 2: ";

		glm::vec3 color{};
		switch (m_CurrentGamemode)
		{
		case LevelManager::GameMode::Coop:
			color = glm::vec3(255.f, 50.f, 200.f);
			break;
		case LevelManager::GameMode::Versus:
			color = glm::vec3(0.f, 50.f, 200.f);
			break;
		}

		auto player2_go = std::make_shared<dae::GameObject>(glm::vec3{ 100.f, 260.f, 0.f });
		player2_go->AddComponent(std::make_shared<dae::TextComponent>(player2_go, text, mediumFont, color));

		auto player2_text_go = std::make_shared<dae::GameObject>(glm::vec3{ 260.f, 260.f, 0.f });
		player2Text = std::make_shared<dae::TextComponent>(player2_text_go, "", mediumFont, glm::vec3(255.f, 255.f, 255.f));
		player2_text_go->AddComponent(player2Text);
	
		scene->Add(player2_go);
		scene->Add(player2_text_go);
	}

	auto highscore_go = std::make_shared<dae::GameObject>();
	auto highscore = std::make_shared<HighScoreComponent>(highscore_go, player1_text_go->GetComponent<dae::TextComponent>(), player2Text);

	highscore_go->AddComponent(highscore);

	text = "Write your name, enter to continue.";
	auto info_go = std::make_shared<dae::GameObject>(glm::vec3{ 150.f, 400.f, 0.f });
	info_go->AddComponent(std::make_shared<dae::TextComponent>(info_go, text, smallFont, glm::vec3(255.f, 255.f, 255.f)));

	highscore->ReadFile("../Data/Highscore.txt");

	scene->Add(score_go);

	scene->Add(player1_go);
	scene->Add(player1_text_go);

	scene->Add(highscore_go);
	scene->Add(info_go);

	dae::ServiceLocator::GetInputManager().AddInput(dae::InputAction(0, dae::ButtonState::downThisFrame, std::make_shared<NameEnteredCommand>(highscore), dae::KeyboardKey::K_Enter));
	dae::ServiceLocator::GetInputManager().AddInput(dae::InputAction(1, dae::ButtonState::downThisFrame, std::make_shared<NameEnteredCommand>(highscore), dae::KeyboardKey::K_Enter));

	return highscore_go;
}

void LevelManager::CreateDisplayHighscore(const std::string& sceneName)
{
	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);

	auto bigFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto mediumFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	std::string text{};
	glm::vec3 textOffset{};
	switch (m_CurrentGamemode)
	{
	case LevelManager::GameMode::Single:
		text = "SINGLEPLAYER HIGHSCORES";
		textOffset = { 60.f, 30.f, 0.f };
		break;
	case LevelManager::GameMode::Coop:
		text = "CO-OP HIGHSCORES";
		textOffset = { 130.f, 40.f, 0.f };
		break;
	case LevelManager::GameMode::Versus:
		text = "VERSUS HIGHSCORES";
		textOffset = { 130.f, 30.f, 0.f };
		break;
	default:
		break;
	}

	auto gamemode_go = std::make_shared<dae::GameObject>(textOffset);
	gamemode_go->AddComponent(std::make_shared<dae::TextComponent>(gamemode_go, text, bigFont, glm::vec3(255.f, 255.f, 10.f)));

	const auto& highscores = m_HighScore->GetComponent<HighScoreComponent>()->GetHighscores();

	int i{};
	for (auto it = highscores.begin(); it != highscores.end(); ++it)
	{
		text = it->second;

		std::string line{};
		std::stringstream stream{text};
		text.clear();

		// Score
		std::string score{};
		std::getline(stream, score, ',');
		std::getline(stream, score, ',');
		
		// Names
		std::getline(stream, line, ',');
		text += std::to_string(i + 1) + ". " + line;

		switch (m_CurrentGamemode)
		{
		case LevelManager::GameMode::Single:
			break;
		case LevelManager::GameMode::Coop:
			std::getline(stream, line, ',');
			text += " & " + line;
			break;
		case LevelManager::GameMode::Versus:
			std::getline(stream, line, ',');
			text += " vs " + line;
			break;
		}

		text += ": " + score;

		auto score_go = std::make_shared<dae::GameObject>(glm::vec3{ 200.f, 90.f + (i * 30.f), 0.f});
		score_go->AddComponent(std::make_shared<dae::TextComponent>(score_go, text, smallFont, glm::vec3(255.f, 255.f, 255.f)));

		scene->Add(score_go);
		++i;
	}

	text = "YOUR HIGHSCORE: " + std::to_string(GameInfo::GetInstance().GetPlayerScore());
	auto highscore_go = std::make_shared<dae::GameObject>(glm::vec3{ 200.f, 400, 0.f });
	highscore_go->AddComponent(std::make_shared<dae::TextComponent>(highscore_go, text, smallFont, glm::vec3(255.f, 255.f, 10.f)));

	text = "A/SPACE to restart";
	auto start_go = std::make_shared<dae::GameObject>(glm::vec3{ 100.f, 440.f, 0.f });
	start_go->AddComponent(std::make_shared<dae::TextComponent>(start_go, text, smallFont, glm::vec3(0.f, 255.f, 100.f)));

	text = "B/ESC to quit";
	auto quit_go = std::make_shared<dae::GameObject>(glm::vec3{ 350.f, 440.f, 0.f });
	quit_go->AddComponent(std::make_shared<dae::TextComponent>(quit_go, text, smallFont, glm::vec3(255.f, 50.f, 50.f)));

	scene->Add(m_HighScore);
	scene->Add(gamemode_go);
	scene->Add(highscore_go);
	scene->Add(start_go);
	scene->Add(quit_go);

	auto quitInput = dae::InputAction(0, dae::ButtonState::releasedThisFrame, std::make_shared<QuitGameCommand>(nullptr), dae::ControllerButton::ButtonB, dae::KeyboardKey::K_Esc);
	dae::ServiceLocator::GetInputManager().AddInput(quitInput);
	auto startInput = dae::InputAction(0, dae::ButtonState::releasedThisFrame, std::make_shared<StartGameCommand>(nullptr), dae::ControllerButton::ButtonA, dae::KeyboardKey::K_SPACE);
	dae::ServiceLocator::GetInputManager().AddInput(startInput);
}

void LevelManager::LoadLevel()
{
	// failsafe incease player dies while scene resets
	switch (m_CurrentGamemode)
	{
	case LevelManager::GameMode::Versus:
	case LevelManager::GameMode::Single:
		if (GameInfo::GetInstance().GetPlayerHealth(0) <= 0)
		{
			LevelFail();
		}
	break;
	case LevelManager::GameMode::Coop:
		if (GameInfo::GetInstance().GetPlayerHealth(0) <= 0 && GameInfo::GetInstance().GetPlayerHealth(1) <= 0)
		{
			LevelFail();
		}
		break;
	default:
		break;
	}

	const std::string& sceneName{ "Level" + std::to_string(m_CurrentLevel) };

	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().CreateScene(sceneName);
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	//dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);

	// LEVEL
	auto level_go = std::make_shared<dae::GameObject>(100.f, 20.f, 0.f);
	std::shared_ptr<LevelLayoutComponent> level_layout = std::make_shared<LevelLayoutComponent>(level_go, m_LevelPahts[m_CurrentLevel], 16, 8);
	auto level_visuals = std::make_shared<LevelVisualComponent>(level_go, level_layout, "Level/wall.png", "Level/void.png", "Level/path.png", "Level/teleport.png");
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
	auto bulletPool_enemy_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, collisionHandler, sceneName, "Sprites/BulletNPC.png", BulletComponent::Type::Enemy, CollisionHandlerComponent::Layer::EnemyBullet, 50);

	bulletPool_player_go->AddComponent(bulletPool_player_comp);
	bulletPool_enemy_go->AddComponent(bulletPool_enemy_comp);

	bulletPool_player_go->SetParent(bulletPool_go, bulletPool_player_go, true);
	bulletPool_enemy_go->SetParent(bulletPool_go, bulletPool_enemy_go, true);

	scene->Add(bulletPool_go);

	// HUD
	auto hud_go = std::make_shared<dae::GameObject>();

	// PLAYER
	std::vector<std::shared_ptr<dae::GameObject>> players{};

	if (GameInfo::GetInstance().GetPlayerHealth(0) > 0)
	{
		auto player_go = LoadPlayer(level_go, hud_go, bulletPool_player_comp, collisionHandler, "Sprites/RedTank.png", "Sprites/RedTankGun.png", 0);
		players.emplace_back(player_go);
	}
	
	std::shared_ptr<dae::GameObject> playerVersus{};

	if (GameInfo::GetInstance().GetPlayerHealth(1) > 0)
	{
		switch (m_CurrentGamemode)
		{
		case LevelManager::GameMode::Single:
			break;
		case LevelManager::GameMode::Coop:
		{
			auto player = LoadPlayer(level_go, hud_go, bulletPool_player_comp, collisionHandler, "Sprites/PinkTank.png", "Sprites/GreenTankGun.png", 1);
			players.emplace_back(player);
			break;
		}
		case LevelManager::GameMode::Versus:
		{
			playerVersus = LoadPlayerVersus(level_go, hud_go, bulletPool_enemy_comp, collisionHandler, "Sprites/BlueTank.png", "Sprites/BlueTankGun.png", 1);
			break;
		}
		default:
			break;
		}
	}

	// SCORE
	auto score_go = std::make_shared<dae::GameObject>(5.f, 60.f, 0.f);
	auto score_comp = std::make_shared<ScoreComponent>(score_go);
	score_comp->SetScore(GameInfo::GetInstance().GetPlayerScore());
	std::string startText = "Score: " + std::to_string(score_comp->GetScore());
	auto scoreDisplay_comp = std::make_shared<ScoreDisplayComponent>(score_go);

	score_go->AddComponent(std::make_shared<dae::TextComponent>(score_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));
	score_go->AddComponent(scoreDisplay_comp);
	score_go->AddComponent(score_comp);

	score_comp->AddObserver(scoreDisplay_comp);
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());
	scoreDisplay_comp->AddObserver(score_go->GetComponent<dae::TextComponent>());

	for (size_t i = 0; i < players.size(); ++i)
	{
		players[i]->GetComponent<HealthComponent>()->AddObserver(score_comp);
	}

	score_go->SetParent(hud_go, score_go, true);

	scene->Add(hud_go);

	// ENEMIES
	auto enemyController = LoadEnemies(scene, level_go, players, bulletPool_enemy_comp, collisionHandler, score_comp);
	for (size_t i = 0; i < players.size(); ++i)
	{
		enemyController->AddObserver(players[i]->GetComponent<HealthComponent>());
		enemyController->AddObserver(players[i]->GetComponent<PlayerComponent>());
	}
	enemyController->AddObserver(score_comp);

	if (m_CurrentGamemode == GameMode::Versus && playerVersus != nullptr)
	{
		enemyController->AddEnemies(playerVersus);
		playerVersus->GetComponent<HealthComponent>()->AddObserver(enemyController);
	}

	for (size_t i{}; i < players.size(); ++i)
	{
		if (auto playerComp = players[i]->GetComponent<PlayerComponent>(); playerComp)
		{
			playerComp->ControllerToRemoveEnemies(enemyController);
		}
	}
}

void LevelManager::StartGame()
{
	m_CurrentLevel = 0;
	GameInfo::GetInstance().SetPlayerScore(0);
	GameInfo::GetInstance().SetPlayerHealth(0, 3);
	GameInfo::GetInstance().SetPlayerHealth(1, 3);

	dae::ServiceLocator::GetInputManager().ClearInput();
	LoadLevel();
	dae::ServiceLocator::GetSceneManager().SetActiveScene("Level" + std::to_string(m_CurrentLevel));
}

void LevelManager::LevelClear()
{
	dae::ServiceLocator::GetInputManager().ClearInput();
	dae::ServiceLocator::GetSceneManager().RemoveScene("Level" + std::to_string(m_CurrentLevel));
	
	++m_CurrentLevel;
	m_CurrentLevel %= m_LevelPahts.size();
	
	LoadLevel();
	dae::ServiceLocator::GetSceneManager().SetActiveScene("Level" + std::to_string(m_CurrentLevel));
}

void LevelManager::LevelFail()
{
	m_HighScore.reset();

	dae::ServiceLocator::GetInputManager().ClearInput();
	dae::ServiceLocator::GetSceneManager().RemoveScene("Level" + std::to_string(m_CurrentLevel));
	
	std::string name{ "EnterName" };
	dae::ServiceLocator::GetSceneManager().RemoveScene(name);
	m_HighScore = CreateEnterName(name);
	dae::ServiceLocator::GetSceneManager().SetActiveScene(name);
}

void LevelManager::NamesEntered()
{
	dae::ServiceLocator::GetInputManager().ClearInput();

	std::string name{ "DisplayHighscore"};
	CreateDisplayHighscore(name);

	dae::ServiceLocator::GetSceneManager().RemoveScene("EnterName");
	dae::ServiceLocator::GetSceneManager().SetActiveScene(name);
}

void LevelManager::AddLevelPath(const std::string& levelPath)
{
	m_LevelPahts.emplace_back(levelPath);
}

LevelManager::GameMode LevelManager::CycleGameMode()
{
	if (m_CurrentGamestate == GameState::MainMenu)
	{
		switch (m_CurrentGamemode)
		{
		case LevelManager::GameMode::Single:
			m_CurrentGamemode = LevelManager::GameMode::Coop;
			break;
		case LevelManager::GameMode::Coop:
			m_CurrentGamemode = LevelManager::GameMode::Versus;
			break;
		case LevelManager::GameMode::Versus:
			m_CurrentGamemode = LevelManager::GameMode::Single;
			break;
		default:
			break;
		}
	}

	return m_CurrentGamemode;
}

LevelManager::GameMode LevelManager::GetGameMode() const
{
	return m_CurrentGamemode;
}

std::shared_ptr<dae::GameObject> LevelManager::LoadPlayer(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler, const std::string& playerPath, const std::string& gunPath, int playerIdx)
{
	dae::BaseInputManager& inputM = dae::ServiceLocator::GetInputManager();

	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();

	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().GetScene("Level" + std::to_string(m_CurrentLevel));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// PLAYER
	const glm::vec2& player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[playerIdx]);
	std::shared_ptr<dae::GameObject> player_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);
	std::shared_ptr<PlayerComponent> player_comp = std::make_shared<PlayerComponent>(player_go, playerIdx);
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
	player_go->GetComponent<HealthComponent>()->AddObserver(player_comp);

	scene->Add(player_go);

	// INPUT
	auto movementInput = dae::InputAction(playerIdx, std::make_shared<MoveCommand>(player_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick, playerIdx), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(playerIdx, std::make_shared<FireCommand>(gun_go->GetComponent<GunComponent>(), dae::Joystick::RightStick, playerIdx), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// HEALTH DISPLAY
	auto healthDisplay_go = std::make_shared<dae::GameObject>(5.f, 10.f + (playerIdx * 20.f), 0.f);
	auto healthDisplay_comp = std::make_shared<HealthDisplayComponent>(healthDisplay_go);
	std::string startText = "Lives: " + std::to_string(player_go->GetComponent<HealthComponent>()->GetCurrentHealth());

	healthDisplay_go->AddComponent(healthDisplay_comp);
	healthDisplay_go->AddComponent(std::make_shared<dae::TextComponent>(healthDisplay_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));

	player_go->GetComponent<HealthComponent>()->AddObserver(healthDisplay_comp);
	healthDisplay_comp->AddObserver(healthDisplay_go->GetComponent<dae::TextComponent>());

	healthDisplay_go->SetParent(hud_go, healthDisplay_go, true);

	return player_go;
}

std::shared_ptr<dae::GameObject> LevelManager::LoadPlayerVersus(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<dae::GameObject> hud_go, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler, const std::string& playerPath, const std::string& gunPath, int playerIdx)
{
	// TODO: VERSUS: make player function like enemy

	dae::BaseInputManager& inputM = dae::ServiceLocator::GetInputManager();

	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();

	dae::Scene* scene = dae::ServiceLocator::GetSceneManager().GetScene("Level" + std::to_string(m_CurrentLevel));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// PLAYER
	const glm::vec2& player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[playerIdx]);
	std::shared_ptr<dae::GameObject> player_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);
	std::shared_ptr<PlayerComponent> player_comp = std::make_shared<PlayerComponent>(player_go, playerIdx);
	player_go->AddComponent(player_comp);

	// VISUALS
	std::shared_ptr<dae::GameObject> visuals_go = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2DComponent> player_texture = std::make_shared<dae::Texture2DComponent>(visuals_go, playerPath);
	player_texture->SetRenderPositionOffset(glm::vec2(player_texture->GetWidth() / 2.f, player_texture->GetHeight() / 2.f));

	visuals_go->AddComponent(player_texture);
	visuals_go->SetParent(player_go, visuals_go, false);

	// MOVEMENT
	player_go->AddComponent(std::make_shared<MoveComponent>(player_go, visuals_go, level_movement, 30.f));

	// GUN + VISUALS
	std::shared_ptr<dae::GameObject> gun_go = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2DComponent> gun_texture = std::make_shared<dae::Texture2DComponent>(gun_go, gunPath);
	gun_texture->SetRenderPositionOffset(glm::vec2(gun_texture->GetWidth() / 2.f, gun_texture->GetHeight() / 2.f));

	gun_go->AddComponent(std::make_shared<GunComponent>(gun_go, bulletPool, BulletComponent::Type::Enemy, 0, 0.5f, 200.f));
	gun_go->AddComponent(gun_texture);
	gun_go->SetParent(player_go, gun_go, false);

	// HEALTH
	player_go->AddComponent(std::make_shared<HealthComponent>(player_go, GameInfo::GetInstance().GetPlayerHealth(playerIdx), 1.f));
	player_go->AddComponent(std::make_shared<CollisionComponent>(player_go, static_cast<float>(player_texture->GetWidth()), static_cast<float>(player_texture->GetHeight())));

	collisionHandler->AddCollider(player_go->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Enemy);

	player_go->GetComponent<CollisionComponent>()->AddObserver(player_comp);
	player_comp->AddObserver(player_go->GetComponent<HealthComponent>());
	player_go->GetComponent<HealthComponent>()->AddObserver(player_comp);

	scene->Add(player_go);

	// INPUT
	auto movementInput = dae::InputAction(playerIdx, std::make_shared<MoveCommand>(player_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick, playerIdx), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(playerIdx, std::make_shared<FireCommand>(gun_go->GetComponent<GunComponent>(), dae::Joystick::RightStick, playerIdx), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// HEALTH DISPLAY
	auto healthDisplay_go = std::make_shared<dae::GameObject>(5.f, 10.f + (playerIdx * 20.f), 0.f);
	auto healthDisplay_comp = std::make_shared<HealthDisplayComponent>(healthDisplay_go);
	std::string startText = "Lives: " + std::to_string(player_go->GetComponent<HealthComponent>()->GetCurrentHealth());

	healthDisplay_go->AddComponent(healthDisplay_comp);
	healthDisplay_go->AddComponent(std::make_shared<dae::TextComponent>(healthDisplay_go, startText, font, glm::vec3{ 255.f, 255.f, 255.f }));

	player_go->GetComponent<HealthComponent>()->AddObserver(healthDisplay_comp);
	healthDisplay_comp->AddObserver(healthDisplay_go->GetComponent<dae::TextComponent>());

	healthDisplay_go->SetParent(hud_go, healthDisplay_go, true);

	return player_go;
}

std::shared_ptr<EnemyControllerComponent> LevelManager::LoadEnemies(dae::Scene* scene, std::shared_ptr<dae::GameObject> level_go, std::vector<std::shared_ptr<dae::GameObject>> player_go, std::shared_ptr<BulletPoolComponent> bulletPool, std::shared_ptr<CollisionHandlerComponent> collisionHandler, std::shared_ptr<ScoreComponent> score_comp)
{
	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();
	std::shared_ptr<LevelPathfindingComponent> level_pathfinding = level_go->GetComponent<LevelPathfindingComponent>();

	// ENEMY
	auto enemyController_go = std::make_shared<dae::GameObject>();
	auto enemyController = std::make_shared<EnemyControllerComponent>(enemyController_go, collisionHandler, player_go);
	enemyController_go->AddComponent(enemyController);

	scene->Add(enemyController_go);

	// BLUE TANKS
	const auto& enemyTank_startPosVec = level_layout->GetEnemyTankStartPositions();
	for (size_t i = 0; i < enemyTank_startPosVec.size(); ++i)
	{
		auto enemy = CreateEnemy(level_go, enemyController, "Sprites/BlueTank.png", enemyTank_startPosVec[i], 30.f);

		std::shared_ptr<GunComponent> gunComp = std::make_shared<GunComponent>(enemy, bulletPool, BulletComponent::Type::Enemy, 0, 1.f, 200.f);
		std::shared_ptr<EnemyTankComponent> enemyComponent = std::make_shared<EnemyTankComponent>(enemy, enemy->GetComponent<MovementControllerComponent>(), gunComp, level_layout, 100);

		enemy->AddComponent(gunComp);
		enemy->AddComponent(enemyComponent);

		collisionHandler->AddCollider(enemy->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Enemy);

		enemyComponent->AddObserver(score_comp);
		enemyComponent->AddObserver(enemy->GetComponent<HealthComponent>());
		enemy->GetComponent<CollisionComponent>()->AddObserver(enemyComponent);
		enemy->GetComponent<HealthComponent>()->AddObserver(enemyController);
		enemy->GetComponent<HealthComponent>()->AddObserver(enemyComponent);

		enemyController->AddEnemies(enemy);
		scene->Add(enemy);
	}

	// RECOGNIZERS
	const auto& enemyRecognizer_startPosVec = level_layout->GetEnemyRecognizerStartPositions();
	for (size_t i = 0; i < enemyRecognizer_startPosVec.size(); ++i)
	{
		// NEW
		auto enemy = CreateEnemy(level_go, enemyController, "Sprites/Recognizer.png", enemyRecognizer_startPosVec[i], 60.f);
		auto enemyComp = std::make_shared<EnemyRecognizerComponent>(enemy, 250);

		enemy->AddComponent(enemyComp);

		collisionHandler->AddCollider(enemy->GetComponent<CollisionComponent>(), CollisionHandlerComponent::Layer::Enemy);

		enemyComp->AddObserver(score_comp);
		enemyComp->AddObserver(enemy->GetComponent<HealthComponent>());
		enemy->GetComponent<CollisionComponent>()->AddObserver(enemyComp);
		enemy->GetComponent<HealthComponent>()->AddObserver(enemyComp);
		enemy->GetComponent<HealthComponent>()->AddObserver(enemyController);

		enemyController->AddEnemies(enemy);

		scene->Add(enemy);
	}

	return enemyController;
}

std::shared_ptr<dae::GameObject> LevelManager::CreateEnemy(std::shared_ptr<dae::GameObject> level_go, std::shared_ptr<EnemyControllerComponent> controller, const std::string& image, int startIdx, float movementSpeed)
{
	std::shared_ptr<LevelLayoutComponent> level_layout = level_go->GetComponent<LevelLayoutComponent>();
	std::shared_ptr<LevelMovementComponent> level_movement = level_go->GetComponent<LevelMovementComponent>();
	std::shared_ptr<LevelPathfindingComponent> level_pathfinding = level_go->GetComponent<LevelPathfindingComponent>();
	
	const glm::vec2& enemy_startPos = level_layout->GetGridCenter(startIdx);
	std::shared_ptr<dae::GameObject> enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);
	
	// VISUALS
	std::shared_ptr<dae::Texture2DComponent> enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, image);
	float textureWidth = static_cast<float>(enemy_texture->GetWidth());
	float textureHeight = static_cast<float>(enemy_texture->GetHeight());
	enemy_texture->SetRenderPositionOffset(glm::vec2(textureWidth / 2.f, textureHeight / 2.f));

	// MOVEMENT
	std::shared_ptr<MoveComponent> enemy_moveComponent = std::make_shared<MoveComponent>(enemy_go, enemy_go, level_movement, movementSpeed);
	std::shared_ptr<MovementControllerComponent> enemy_moveController = std::make_shared<MovementControllerComponent>(enemy_go, controller, enemy_moveComponent, level_pathfinding, level_layout);

	// HEALTH + COLLISION
	std::shared_ptr<HealthComponent> enemy_healthComponent = std::make_shared<HealthComponent>(enemy_go, 3, 0.5f);
	std::shared_ptr<CollisionComponent> enemy_collider = std::make_shared<CollisionComponent>(enemy_go, textureWidth, textureHeight);

	enemy_go->AddComponent(enemy_texture);
	enemy_go->AddComponent(enemy_moveComponent);
	enemy_go->AddComponent(enemy_moveController);
	enemy_go->AddComponent(enemy_healthComponent);
	enemy_go->AddComponent(enemy_collider);

	return enemy_go;
}
