#pragma once
#include <BaseComponent.h>

class CollisionComponent;

class CollisionHandlerComponent : public dae::BaseComponent
{
public:
	enum class Layer
	{
		Player,
		Enemy,
		PlayerBullet,
		EnemyBullet,
		NumberOfElements
	};

	CollisionHandlerComponent(std::shared_ptr<dae::GameObject> gameObject);
	~CollisionHandlerComponent() = default;

	void Update() override;
	void Render() const override {};

	void AddCollider(std::shared_ptr<CollisionComponent> collider, Layer layer);
	void AddCollisionIgnore(Layer layer1, Layer layer2);
	void RemoveCollisionIgnore(Layer layer1, Layer layer2);

private:
	bool IsOverlapping(const glm::vec2& l1, const glm::vec2& r1, const glm::vec2& l2, const glm::vec2& r2);

	std::vector<std::vector<CollisionComponent*>> m_CollidersPerLayer;
	std::vector<std::vector<Layer>> m_CollisionWithLayers;
};

