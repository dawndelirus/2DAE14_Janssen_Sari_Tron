#pragma once
#include <BaseComponent.h>

class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(std::shared_ptr<dae::GameObject> gameObject, float movementSpeed);
	~MoveComponent() = default;

	void Update() override {};
	void Render() const override {};

	void Move(float x, float y, float magnitude);

private:
	float m_MovementSpeed;

};

