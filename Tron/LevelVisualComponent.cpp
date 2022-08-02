#include "LevelVisualComponent.h"
#include "LevelLayoutComponent.h"
#include "Texture2DComponent.h"

LevelVisualComponent::LevelVisualComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_LevelLayout{levelLayout}
{
	//CreateVisuals();
}

void LevelVisualComponent::CreateVisuals()
{
	std::shared_ptr<LevelLayoutComponent> levelLayout = m_LevelLayout.lock();

	auto levelVisuals = std::make_shared<dae::GameObject>();

	for (size_t i = 0; i < levelLayout->GetGridSize(); ++i)
	{
		auto visualChild = std::make_shared<dae::GameObject>();
		std::shared_ptr<dae::Texture2DComponent>texComp{};

		if (levelLayout->IsWalkable(static_cast<int>(i)))
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Level/yes.png");
		}
		else
		{
			texComp = std::make_shared<dae::Texture2DComponent>(visualChild, "Level/no.png");
		}

		auto pos = levelLayout->GetGridCenter(i);

		visualChild->AddComponent(texComp);
		visualChild->SetParent(levelVisuals, visualChild, true);
		visualChild->SetLocalPosition(pos.x, pos.y, 0.f);
	}
	levelVisuals->SetParent(GetGameObject(), levelVisuals, false);
}