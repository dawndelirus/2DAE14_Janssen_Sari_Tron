#pragma once
#include <BaseComponent.h>
#include "BulletPoolComponent.h"

class GunComponent final : public dae::BaseComponent
{
public:
	GunComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<BulletPoolComponent> bulletPool, BulletComponent:: Type bulletType, int bounces, float fireCooldown, float bulletSpeed);
	~GunComponent() = default;

	void ShootBullet(const glm::vec2& direction);

	void Update() override;
	void Render() const override {};

private:
	std::shared_ptr<BulletPoolComponent> m_BulletPool;
	BulletComponent::Type m_BulletType;
	float m_FireCooldown;
	float m_CurrentFireCooldown;
	float m_BulletSpeed;
	int m_Bounces;
};

