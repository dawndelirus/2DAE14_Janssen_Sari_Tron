#pragma once
#include "BaseComponent.h"

class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(std::weak_ptr<dae::GameObject> gameObject, float movementSpeed);
	~MoveComponent() = default;

	void Update() override;
	void Render() const override {};

	void Move(glm::vec2 direction);
	glm::vec2& GetDirection() { return m_MoveDirection; };
private:
	float m_MovementSpeed;
	glm::vec2 m_MoveDirection;
};

