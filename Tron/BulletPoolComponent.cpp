#include "BulletPoolComponent.h"
#include "GameObject.h"
#include "BulletComponent.h"
#include <iostream>
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "LevelLayoutComponent.h"

BulletPoolComponent::BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout, const std::string& sceneName, int bulletAmount)
	: BaseComponent(gameObject)
	, m_FirstAvailable{}
	, m_Bullets{}
{
	auto pScene = dae::ServiceLocator::GetSceneManager().GetScene(sceneName);
	
	std::shared_ptr<BulletComponent> previous{};

	for (size_t i = 0; i < bulletAmount; ++i)
	{
		auto bullet_go = std::make_shared<dae::GameObject>();
		std::shared_ptr<BulletComponent> bulletComponent{ new BulletComponent(bullet_go, levelLayout) }; // new because can not create object from friend class using RAII
		bullet_go->AddComponent(bulletComponent);
		auto textureComponent = std::make_shared<dae::Texture2DComponent>(bullet_go, "Sprites/BulletPlayer.png");
		textureComponent->SetRenderPositionOffset(glm::vec2(16.f, 16.f));
		bullet_go->AddComponent(textureComponent);

		if (i == 0)
		{
			m_FirstAvailable = bulletComponent.get();
			previous = bulletComponent;
		}
		else
		{
			bulletComponent->SetNext(nullptr);
			previous->SetNext(bulletComponent.get());
			previous = bulletComponent;
		}

		m_Bullets.emplace_back(bullet_go.get());
		pScene->Add(bullet_go);
	}
}

void BulletPoolComponent::CreateBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed, BulletComponent::Type bulletSource)
{
	if (m_FirstAvailable == nullptr)
	{
		return;
	}
	std::cout << "FIRE!\n";

	BulletComponent* newBullet = m_FirstAvailable;
	m_FirstAvailable = newBullet->GetNext();

	newBullet->InitializeBullet(startPos, direction, bounces, bulletSpeed, bulletSource);
}

void BulletPoolComponent::Update()
{
	for (size_t i = 0; i < m_Bullets.size(); ++i)
	{
		auto comp = m_Bullets[i]->GetComponent<BulletComponent>();
		if (!comp->IsInUse())
		{
			comp->SetNext(m_FirstAvailable);
			m_FirstAvailable = comp.get();
		}
	}
}
