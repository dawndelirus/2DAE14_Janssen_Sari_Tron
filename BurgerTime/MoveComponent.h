#pragma once
#include "BaseComponent.h"

class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(std::weak_ptr<dae::GameObject> gameObject, float movementSpeed);
	~MoveComponent() = default;

	void Update();
	void Render() const {};

	void Move(glm::vec2 direction);
private:
	float m_MovementSpeed;
	glm::vec2 m_MoveDirection;
};

