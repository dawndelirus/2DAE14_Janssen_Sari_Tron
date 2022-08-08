#include "BulletPoolComponent.h"

BulletPoolComponent::BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, int bulletAmount)
	: BaseComponent(gameObject)
{
	for (size_t i = 0; i < bulletAmount; ++i)
	{
		// TODO: BULLETPOOL: Initialize bullets
		//m_Bullets.emplace_back()
	}
}

void BulletPoolComponent::CreateBullet(glm::vec2 startPos, glm::vec2 direction, int bounces, BulletComponent::Source bulletSource)
{
	BulletComponent* newBullet = m_FirstAvailable;
	m_FirstAvailable = newBullet->GetNext();

	newBullet->InitializeBullet(startPos, direction, bounces, bulletSource);
}
