#include "BurgerTimePCH.h"
#include "LevelPrefab.h"
#include "BinToCharVec.h"
#include "Texture2DComponent.h"


glm::vec2 GetGridTopLeft(size_t idx)
{
	glm::vec2 gridPos{};
	gridPos.x = static_cast<float>(static_cast<int>(idx) % 17 * 24);
	gridPos.y = static_cast<float>(static_cast<int>(idx) / 17 * 32);
	return gridPos;
}

glm::vec2 GetGridBottomCenter(size_t idx)
{
	auto pos = GetGridTopLeft(idx);
	pos.x += 12.f;
	pos.y += 32.f;
	return pos;
}

std::shared_ptr<dae::GameObject> LoadLevelVisuals(std::vector<char> output)
{
	auto levelVisuals = std::make_shared<dae::GameObject>(0, 0, 0);
	for (size_t i = 0; i < output.size(); ++i)
	{
		if (static_cast<int>(output[i]) == 4)
		{
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
		}

		auto pos = GetGridTopLeft(i);

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition(pos.x, pos.y, 0.f);

	}

	return levelVisuals;
}

std::shared_ptr<dae::GameObject> LoadLevelFood(std::vector<char> output)
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

	return levelVisuals;
}

std::unordered_multimap<int, glm::vec2> GetPlayerPositions(std::vector<char> output)
{
	std::unordered_multimap<int, glm::vec2> positions;

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
		}

		pos.x -= 32.f / 2.f;
		pos.y -= 32.f;
		positions.emplace(std::make_pair(index, pos));
	}
	return positions;
}

std::unordered_multimap<int, glm::vec2> GetEnemyPositions(std::vector<char> output)
{
	std::unordered_multimap<int, glm::vec2> positions;

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
		}

		pos.x -= 32.f / 2.f;
		pos.y -= 32.f;
		positions.emplace(std::make_pair(index, pos));
	}
	return positions;
}

void LoadLevel(dae::Scene* scene)
{
	std::vector<char> output{};
	dae::BinToCharVec(output, "../Data/Level/Level0.bin");

	auto itVisuals = std::find(output.begin(), output.end(), 0);
	auto itFood = std::find(output.begin(), output.end(), 1);
	auto itPlayers = std::find(output.begin(), output.end(), 2);
	auto itEnemies = std::find(output.begin(), output.end(), 3);

	auto levelObject = std::make_shared<dae::GameObject>(100, 50, 0);


	std::vector<char> outputVisuals{ ++itVisuals, itFood };
	auto level = LoadLevelVisuals(outputVisuals);
	level->SetParent(levelObject, level, false);

	std::vector<char> outputFood{ ++itFood, itPlayers };
	level = LoadLevelFood(outputFood);
	level->SetParent(levelObject, level, false);

	std::vector<char> outputPlayers{ ++itPlayers, itEnemies };
	auto players = GetPlayerPositions(outputPlayers);
	players;

	std::vector<char> outputEnemies{ ++itEnemies, output.end()};
	auto enemies = GetEnemyPositions(outputEnemies);
	enemies;

	scene->Add(levelObject);
}
