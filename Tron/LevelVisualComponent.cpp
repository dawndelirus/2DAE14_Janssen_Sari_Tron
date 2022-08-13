#include "LevelVisualComponent.h"
#include "LevelLayoutComponent.h"
#include "Texture2DComponent.h"

LevelVisualComponent::LevelVisualComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout, const std::string& wallFile, const std::string& voidFile, const std::string& pathFile)
	: BaseComponent(gameObject)
	, m_LevelLayout(levelLayout)
{
	CreateVisuals(wallFile, voidFile, pathFile);
}

void LevelVisualComponent::CreateVisuals(const std::string& wallFile, const std::string& voidFile, const std::string& pathFile)
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
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, pathFile);
		}
		else if(vec[static_cast<int>(i)] == 0)
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, wallFile);
		}
		else if(vec[static_cast<int>(i)] == 1)
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, voidFile);
		}

		texComp->SetRenderPositionOffset(glm::vec2(texComp->GetWidth() / 2.f, texComp->GetHeight() / 2.f));

		auto pos = levelLayout->GetGridCenterVisuals(static_cast<int>(i));

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition({ pos.x, pos.y, 0.f });
	}
	levelVisuals->SetParent(GetGameObject(), levelVisuals, true);
}