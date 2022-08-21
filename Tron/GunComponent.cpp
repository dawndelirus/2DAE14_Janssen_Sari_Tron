#include "GunComponent.h"
#include "Clock.h"
#include "ServiceLocator.h"

GunComponent::GunComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<BulletPoolComponent> bulletPool, BulletComponent::Type bulletType, int bounces, float fireCooldown, float bulletSpeed)
	: BaseComponent(gameObject)
	, m_BulletPool(bulletPool)
	, m_BulletType(bulletType)
	, m_ShootDirection()
	, m_FireCooldown(fireCooldown)
	, m_CurrentFireCooldown(fireCooldown)
	, m_BulletSpeed(bulletSpeed)
	, m_Bounces(bounces)
	, m_ShotBullet{ false }
{
}

void GunComponent::Update()
{
	if (m_CurrentFireCooldown > 0.f)
	{
		m_CurrentFireCooldown -= dae::Clock::GetDeltaTime();
	}

	if (m_CurrentFireCooldown <= 0.f && m_ShotBullet)
	{
		m_ShootDirection.y *= -1.f;
		m_BulletPool->CreateBullet(GetGameObject()->GetWorldPosition(), m_ShootDirection, m_Bounces, m_BulletSpeed);
		m_CurrentFireCooldown = m_FireCooldown;
		dae::ServiceLocator::GetSoundSystem().PlaySound(0, 1);

		m_ShotBullet = false;
	}

	m_ShootDirection = glm::vec2{};
}

void GunComponent::ShootBullet(const glm::vec2& direction)
{
	m_ShootDirection += direction;

	m_ShootDirection = glm::normalize(m_ShootDirection);

	float angle = glm::orientedAngle(glm::vec2(m_ShootDirection.x, m_ShootDirection.y), glm::vec2(1.f, 0.f));
	GetGameObject()->SetLocalRotation(glm::degrees(angle));

	m_ShotBullet = true;
}
