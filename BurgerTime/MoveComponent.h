#pragma once
#include "BaseComponent.h"

class LevelMovementComponent;
class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(std::weak_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelMovementComponent> movementLevel, float movementSpeed);
	~MoveComponent() = default;

	void Update() override;
	void Render() const override {};

	void Move(glm::vec2 direction);
	glm::vec2& GetDirection() { return m_MoveDirection; };
private:
	std::weak_ptr<LevelMovementComponent> m_MovementLevel;
	float m_MovementSpeed;
	glm::vec2 m_MoveDirection;
};

