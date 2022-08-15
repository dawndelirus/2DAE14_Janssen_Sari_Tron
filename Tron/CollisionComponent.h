#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class CollisionComponent : public dae::BaseComponent, public dae::Subject
{
public:
	CollisionComponent(std::shared_ptr<dae::GameObject> gameObject, float width, float height);
	~CollisionComponent() = default;

	void Update() override {};
	void Render() const override {};

	void GetHit();

	glm::vec2 GetPosition() const;
	float GetWidth();
	float GetHeight();

private:
	float m_Width;
	float m_Height;
};

