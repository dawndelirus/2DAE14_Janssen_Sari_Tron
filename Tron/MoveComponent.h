#pragma once
#include <BaseComponent.h>

class LevelMovementComponent;

class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelMovementComponent> levelMovement, float movementSpeed);
	~MoveComponent() = default;

	void Update() override {};
	void Render() const override {};

	void Move(float x, float y, float magnitude);

private:
	std::weak_ptr<LevelMovementComponent> m_LevelMovement;
	float m_MovementSpeed;
};

