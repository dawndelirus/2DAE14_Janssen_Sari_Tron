#pragma once
#include <BaseComponent.h>

class CollisionComponent;

class CollisionHandlerComponent : public dae::BaseComponent
{
public:
	CollisionHandlerComponent(std::shared_ptr<dae::GameObject> gameObject);
	~CollisionHandlerComponent() = default;

	void Update() override;
	void Render() const override {};

private:
	bool IsOverlapping(const glm::vec2& l1, const glm::vec2& r1, const glm::vec2& l2, const glm::vec2& r2);

	std::vector<std::vector<CollisionComponent*>> m_CollisionLayers;
};

