#include "LevelVisualComponent.h"
#include "LevelLayoutComponent.h"
#include "Texture2DComponent.h"

LevelVisualComponent::LevelVisualComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_LevelLayout{levelLayout}
{
	CreateVisuals();
}

void LevelVisualComponent::CreateVisuals()
{
	std::shared_ptr<LevelLayoutComponent> levelLayout = m_LevelLayout.lock();

	auto levelVisuals = std::make_shared<dae::GameObject>();

	auto& vec = levelLayout->GetVisualsVector();

	for (size_t i = 0; i < levelLayout->GetVisualsGridSize(); ++i)
	{
		auto visualChild = std::make_shared<dae::GameObject>();
		std::shared_ptr<dae::Texture2DComponent>texComp{};

		if (vec[static_cast<int>(i)] >= 2)
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Level/path.png");
		}
		else if(vec[static_cast<int>(i)] == 0)
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Level/wall.png");
		}
		else if(vec[static_cast<int>(i)] == 1)
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Level/void.png");
		}

		float tileWidth = static_cast<float>(levelLayout->GetVisualTileWidth());
		texComp->SetRenderPositionOffset(glm::vec2(tileWidth / 2.f, tileWidth / 2.f));

		auto pos = levelLayout->GetGridCenterVisuals(static_cast<int>(i));

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition({ pos.x, pos.y, 0.f });
	}
	levelVisuals->SetParent(GetGameObject(), levelVisuals, true);
}