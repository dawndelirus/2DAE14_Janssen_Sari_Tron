#include "BurgerTimePCH.h"
#include "LevelPrefab.h"

glm::vec2 GetGridPosition(size_t idx, int width, int height)
{
	glm::vec2 gridPos{};
	gridPos.x = static_cast<float>(static_cast<int>(idx) % 17 * width);
	gridPos.y = static_cast<float>(static_cast<int>(idx) / 17 * height);
	return gridPos;
}

std::shared_ptr<dae::GameObject> LoadLevelVisuals(std::vector<char> output)
{
	auto levelVisuals = std::make_shared<dae::GameObject>(0, 0, 0);
	for (size_t i = 0; i < output.size(); ++i)
	{
		switch (static_cast<int>(output[i]))
		{
		case 5:
		{
			auto visualChild = std::make_shared<dae::GameObject>(0, 0, 0);
			auto texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/Ladder.png");
			visualChild->AddComponent(texComp);
			visualChild->SetParent(levelVisuals, visualChild, true);

			auto pos = GetGridPosition(i, texComp->GetWidth(), texComp->GetHeight());

			visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
			break;
		}
		case 6:
		{
			auto visualChild = std::make_shared<dae::GameObject>(0, 0, 0);
			auto texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/Platform.png");
			visualChild->AddComponent(texComp);
			visualChild->SetParent(levelVisuals, visualChild, true);

			auto pos = GetGridPosition(i, texComp->GetWidth(), texComp->GetHeight());

			visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
			break;
		}
		case 7:
		{
			auto visualChild = std::make_shared<dae::GameObject>(0, 0, 0);
			auto texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Sprites/Level/LadderPlatform.png");
			visualChild->AddComponent(texComp);
			visualChild->SetParent(levelVisuals, visualChild, true);

			auto pos = GetGridPosition(i, texComp->GetWidth(), texComp->GetHeight());

			visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
			break;
		}
		}
	}

	return levelVisuals;
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

	scene->Add(levelObject);
}
