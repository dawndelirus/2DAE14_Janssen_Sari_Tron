#pragma once
#include <BaseComponent.h>
#include "BulletComponent.h"

class BulletPoolComponent final : public dae::BaseComponent
{
public:
	BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, int bulletAmount);
	~BulletPoolComponent() = default;

	//void CreateBullet(glm::vec2 startPos, glm::vec2 direction, int bounces, BulletComponent::Source bulletSource);

	void Update() override {};
	void Render() const override {};

private:
	//BulletComponent* m_FirstAvailable;

	std::vector<dae::GameObject> m_Bullets;

};

