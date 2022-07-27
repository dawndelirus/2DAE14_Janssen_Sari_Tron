#pragma once
#include <BaseComponent.h>

class MoveComponent final : public dae::BaseComponent
{
public:
	MoveComponent(dae::GameObject* pGameObject);
	~MoveComponent() = default;

	void Update() override {};
	void Render() const override {};

	void Move(float x, float y, float magnitude);
};

