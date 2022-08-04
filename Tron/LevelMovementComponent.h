#pragma once
#include "BaseComponent.h"
#include <memory>

class LevelLayoutComponent;

class LevelMovementComponent final : public dae::BaseComponent
{
public:
	LevelMovementComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout);
	~LevelMovementComponent() = default;

	void MoveOnGrid(glm::vec3& position, const glm::vec2& displacement);

	void Update() override {};
	void Render() const override {};

private:
	void MoveToGridCenter();
	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;
};
