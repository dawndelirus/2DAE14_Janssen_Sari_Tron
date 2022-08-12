#include "GunComponent.h"
#include "Clock.h"

GunComponent::GunComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<BulletPoolComponent> bulletPool, BulletComponent::Type bulletType, int bounces, float fireCooldown, float bulletSpeed)
	: BaseComponent(gameObject)
	, m_BulletPool(bulletPool)
	, m_BulletType(bulletType)
	, m_FireCooldown(fireCooldown)
	, m_CurrentFireCooldown(fireCooldown)
	, m_BulletSpeed(bulletSpeed)
	, m_Bounces(bounces)
{
}

void GunComponent::Update()
{
	if (m_CurrentFireCooldown > 0.f)
	{
		m_CurrentFireCooldown -= dae::Clock::GetDeltaTime();
	}
}

void GunComponent::ShootBullet(const glm::vec2& direction)
{
	float angle = glm::orientedAngle(glm::vec2(direction.x, -direction.y), glm::vec2(1.f, 0.f));
	GetGameObject()->SetLocalRotation(glm::degrees(angle));

	if (m_CurrentFireCooldown <= 0.f)
	{
		m_BulletPool->CreateBullet(GetGameObject()->GetWorldPosition(), direction, m_Bounces, m_BulletSpeed, m_BulletType);
		m_CurrentFireCooldown = m_FireCooldown;
	}
}
