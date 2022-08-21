#include "CollisionHandlerComponent.h"
#include "CollisionComponent.h"

CollisionHandlerComponent::CollisionHandlerComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_CollidersPerLayer{}
	, m_CollisionWithLayers{}
{
	m_CollidersPerLayer.resize(static_cast<int>(Layer::NumberOfElements));
	m_CollisionWithLayers.resize(static_cast<int>(Layer::NumberOfElements));

	for (size_t i = 0; i < static_cast<int>(Layer::NumberOfElements); ++i)
	{
		for (size_t j = 0; j < static_cast<int>(Layer::NumberOfElements); ++j)
		{
			if (i == j)
			{
				continue;
			}
			m_CollisionWithLayers[i].emplace_back(static_cast<Layer>(j));
		}
	}
}

void CollisionHandlerComponent::Update()
{
	for (size_t i{}; i < m_CollidersPerLayer.size(); ++i)
	{
		for (size_t j{}; j < m_CollisionWithLayers[i].size(); ++j)
		{
			// Avoid checking colision with layers that have been checked before
			size_t collisionIndexToCheck = static_cast<size_t>(m_CollisionWithLayers[i][j]);
			if (i >= collisionIndexToCheck)
			{
				continue;
			}

			auto collisionVector0 = m_CollidersPerLayer[i];
			auto collisionVector1 = m_CollidersPerLayer[collisionIndexToCheck];

			// Loop over all the colliders in the collision layers
			for (auto layer0 : collisionVector0)
			{
				for (auto layer1 : collisionVector1)
				{
					if (IsOverlapping({ layer0->GetPosition().x - layer0->GetWidth() / 2.f, layer0->GetPosition().y + layer0->GetHeight() / 2.f }
						, { layer0->GetPosition().x + layer0->GetWidth() / 2.f, layer0->GetPosition().y - layer0->GetHeight() / 2.f }
						, { layer1->GetPosition().x - layer1->GetWidth() / 2.f, layer1->GetPosition().y + layer1->GetHeight() / 2.f }
						, { layer1->GetPosition().x + layer1->GetWidth() / 2.f, layer1->GetPosition().y - layer1->GetHeight() / 2.f }))
					{
						layer0->GetHit(static_cast<CollisionHandlerComponent::Layer>(collisionIndexToCheck));
						layer1->GetHit(static_cast<CollisionHandlerComponent::Layer>(i));
					}
				}
			}
		}
	}
}

void CollisionHandlerComponent::AddCollider(std::shared_ptr<CollisionComponent> collider, Layer layer)
{
	m_CollidersPerLayer[static_cast<int>(layer)].emplace_back(collider.get());
}

void CollisionHandlerComponent::RemoveCollider(std::shared_ptr<CollisionComponent> collider, Layer layer)
{
	auto it = std::find(m_CollidersPerLayer[static_cast<int>(layer)].begin(), m_CollidersPerLayer[static_cast<int>(layer)].end(), collider.get());
	if (it != m_CollidersPerLayer[static_cast<int>(layer)].end())
	{
		m_CollidersPerLayer[static_cast<int>(layer)].erase(it);
	}
}

void CollisionHandlerComponent::AddCollisionIgnore(Layer layer1, Layer layer2)
{
	auto it = std::find(m_CollisionWithLayers[static_cast<int>(layer1)].begin(), m_CollisionWithLayers[static_cast<int>(layer1)].end(), layer2);
	if (it != m_CollisionWithLayers[static_cast<int>(layer1)].end())
	{
		m_CollisionWithLayers[static_cast<int>(layer1)].erase(it);
	}

	it = std::find(m_CollisionWithLayers[static_cast<int>(layer2)].begin(), m_CollisionWithLayers[static_cast<int>(layer2)].end(), layer1);
	if (it != m_CollisionWithLayers[static_cast<int>(layer2)].end())
	{
		m_CollisionWithLayers[static_cast<int>(layer2)].erase(it);
	}
}

void CollisionHandlerComponent::RemoveCollisionIgnore(Layer layer1, Layer layer2)
{
	auto it = std::find(m_CollisionWithLayers[static_cast<int>(layer1)].begin(), m_CollisionWithLayers[static_cast<int>(layer1)].end(), layer2);
	if (it == m_CollisionWithLayers[static_cast<int>(layer1)].end())
	{
		m_CollisionWithLayers[static_cast<int>(layer1)].emplace_back(layer2);
	}

	it = std::find(m_CollisionWithLayers[static_cast<int>(layer2)].begin(), m_CollisionWithLayers[static_cast<int>(layer2)].end(), layer1);
	if (it == m_CollisionWithLayers[static_cast<int>(layer2)].end())
	{
		m_CollisionWithLayers[static_cast<int>(layer2)].emplace_back(layer1);
	}
}

bool CollisionHandlerComponent::IsOverlapping(const glm::vec2& LT1, const glm::vec2& RB1, const glm::vec2& LT2, const glm::vec2& RB2)
{
	// if rectangle has area 0, no overlap
	if (LT1.x == RB1.x || LT1.y == RB1.y || RB2.x == LT2.x || LT2.y == RB2.y)
		return false;

	// If one rectangle is on left side of other
	if (LT1.x > RB2.x || LT2.x > RB1.x)
		return false;

	// If one rectangle is above other
	if (RB1.y > LT2.y || RB2.y > LT1.y)
		return false;

	return true;
}
