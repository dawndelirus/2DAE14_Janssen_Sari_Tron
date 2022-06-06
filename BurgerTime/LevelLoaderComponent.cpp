#include "BurgerTimePCH.h"
#include "LevelLoaderComponent.h"
#include "Texture2DComponent.h"
#include "BinToCharVec.h"
#include "LevelMovementComponent.h"

LevelLoaderComponent::LevelLoaderComponent(std::weak_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelMovementComponent> walkableLevel, int gridWidth)
	: BaseComponent(gameObject)
	, m_TileWidth{}
	, m_TileHeight{}
	, m_GridWidth{ gridWidth }
	, m_LevelMovement{walkableLevel}
{
	walkableLevel->SetGridWidth(gridWidth);
}

glm::vec2 LevelLoaderComponent::GetGridTopLeft(size_t idx)
{
	glm::vec2 gridPos{};
	gridPos.x = static_cast<float>(static_cast<int>(idx) % m_GridWidth * m_TileWidth);
	gridPos.y = static_cast<float>(static_cast<int>(idx) / m_GridWidth * m_TileHeight);
	return gridPos;
}

glm::vec2 LevelLoaderComponent::GetGridBottomCenter(size_t idx)
{
	auto pos = GetGridTopLeft(idx);
	pos.x += m_TileWidth / 2.f;
	pos.y += m_TileHeight;
	return pos;
}

void LevelLoaderComponent::LoadLevel(const std::string& levelPath)
{
	std::vector<char> output{};
	dae::BinToCharVec(output, levelPath);

	auto itVisuals = std::find(output.begin(), output.end(), 0);
	auto itFood = std::find(output.begin(), output.end(), 1);
	auto itPlayers = std::find(output.begin(), output.end(), 2);
	auto itEnemies = std::find(output.begin(), output.end(), 3);

	std::vector<char> outputVisuals{ ++itVisuals, itFood };
	LoadLevelVisuals(outputVisuals);

	std::vector<char> outputFood{ ++itFood, itPlayers };
	LoadLevelFood(outputFood);

	std::vector<char> outputPlayers{ ++itPlayers, itEnemies };
	LoadPlayerPositions(outputPlayers);

	std::vector<char> outputEnemies{ ++itEnemies, output.end() };
	LoadEnemyPositions(outputEnemies);
}

const std::unordered_map<int, glm::vec2>& LevelLoaderComponent::GetPlayerPositions() const
{
	return m_PlayerPositions;
}

void LevelLoaderComponent::LoadLevelVisuals(std::vector<char> output)
{
	auto levelVisuals = std::make_shared<dae::GameObject>(0, 0, 0);
	std::vector<bool> isWalkable{};

	for (size_t i = 0; i < output.size(); ++i)
	{
		if (static_cast<int>(output[i]) == 4)
		{
			isWalkable.emplace_back(false);
			continue;
		}

		auto visualChild = std::make_shared<dae::GameObject>(0, 0, 0);
		std::shared_ptr<dae::Texture2DComponent>texComp{};

		switch (static_cast<int>(output[i]))
		{
		case 5:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/Ladder.png");
			break;
		case 6:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/Platform.png");
			break;
		case 7:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/LadderPlatform.png");
			break;
		default:
			std::cout << "Case without a way out found\n\n";
			break;
		}

		if (m_TileWidth == 0.f || m_TileHeight == 0.f)
		{
			m_TileWidth = texComp->GetWidth();
			m_TileHeight = texComp->GetHeight();
			m_LevelMovement.lock()->SetTileSize(glm::vec2(m_TileWidth, m_TileHeight));
		}

		auto pos = GetGridTopLeft(i);

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
		isWalkable.emplace_back(true);
	}

	levelVisuals->SetParent(GetGameObject(), levelVisuals, false);
	m_LevelMovement.lock()->SetWalkableGrid(isWalkable);
}

void LevelLoaderComponent::LoadLevelFood(std::vector<char> output)
{
	auto levelVisuals = std::make_shared<dae::GameObject>(0, 0, 0);
	for (size_t i = 0; i < output.size(); ++i)
	{
		if (static_cast<int>(output[i]) == 4)
		{
			continue;
		}

		std::shared_ptr<dae::GameObject> visualChild{ std::make_shared<dae::GameObject>(0, 0, 0) };
		std::shared_ptr<dae::Texture2DComponent> texComp{};

		switch (static_cast<int>(output[i]))
		{
		case 5:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Bun_Top_Full.png");
			break;
		case 6:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Bun_Bottom_Full.png");
			break;
		case 7:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Lettuce_Full.png");
			break;
		case 8:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Tomato_Full.png");
			break;
		case 9:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Meat_Full.png");
			break;
		case 10:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Burger/Cheese_Full.png");
			break;
		case 11:
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/Plate.png");
			break;
		default:
			std::cout << "Case without a way out found\n\n";
			break;
		}

		auto pos = GetGridBottomCenter(i);
		pos.x -= texComp->GetWidth() / 2.f;
		pos.y -= texComp->GetHeight();

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
	}

	levelVisuals->SetParent(GetGameObject(), levelVisuals, false);
}

void LevelLoaderComponent::LoadPlayerPositions(std::vector<char> output)
{
	for (size_t i = 0; i < output.size(); ++i)
	{
		if (static_cast<int>(output[i]) == 4)
		{
			continue;
		}

		glm::vec2 pos{};

		int index = static_cast<int>(output[i]);
		switch (index)
		{
		case 5:
			pos = GetGridBottomCenter(i);
			break;
		case 6:
			pos = GetGridBottomCenter(i);
			break;
		case 7:
			pos = GetGridBottomCenter(i);
			break;
		default:
			std::cout << "Case without a way out found\n\n";
			break;
		}

		m_PlayerPositions.emplace(std::make_pair(index, pos));
	}
}

void LevelLoaderComponent::LoadEnemyPositions(std::vector<char> output)
{
	for (size_t i = 0; i < output.size(); ++i)
	{
		if (static_cast<int>(output[i]) == 4)
		{
			continue;
		}

		glm::vec2 pos{};

		int index = static_cast<int>(output[i]);
		switch (index)
		{
		case 5:
			pos = GetGridBottomCenter(i);
			break;
		case 6:
			pos = GetGridBottomCenter(i);
			break;
		case 7:
			pos = GetGridBottomCenter(i);
			break;
		default:
			std::cout << "Case without a way out found\n\n";
			break;
		}

		pos.x -= 32.f / 2.f;
		pos.y -= 32.f;
		m_EnemyPositions.emplace(std::make_pair(index, pos));
	}
}
