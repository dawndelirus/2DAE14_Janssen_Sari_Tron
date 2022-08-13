#pragma once
#include <BaseComponent.h>
#include <string>
#include "BulletComponent.h"

class LevelLayoutComponent;

class BulletPoolComponent final : public dae::BaseComponent
{
public:
	BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout
		, const std::string& sceneName, const std::string& texturePath, BulletComponent::Type bulletSource, int bulletAmount);
	~BulletPoolComponent() = default;

	void CreateBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed);

	void Update() override;
	void Render() const override {};

private:
	BulletComponent* m_FirstAvailable;

	std::vector<BulletComponent*> m_Bullets;
	BulletComponent::Type m_BulletType;
};

