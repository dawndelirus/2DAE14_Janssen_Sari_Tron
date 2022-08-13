#include "CollisionHandlerComponent.h"
#include "CollisionComponent.h"

CollisionHandlerComponent::CollisionHandlerComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_CollisionLayers{}
{
}

void CollisionHandlerComponent::Update()
{
	for (size_t i{}; i < m_CollisionLayers.size() - 1; ++i)
	{
		for (auto layer0 : m_CollisionLayers[i])
		{
			for (auto layer1 : m_CollisionLayers[i + 1])
			{
				if (IsOverlapping({layer0->GetPosition().x - layer0->GetWidth() / 2.f, layer0->GetPosition().y + layer0->GetHeight() / 2.f}
						, { layer0->GetPosition().x + layer0->GetWidth() / 2.f, layer0->GetPosition().y - layer0->GetHeight() / 2.f }
						, { layer1->GetPosition().x - layer1->GetWidth() / 2.f, layer1->GetPosition().y + layer1->GetHeight() / 2.f }
						, { layer1->GetPosition().x + layer1->GetWidth() / 2.f, layer1->GetPosition().y - layer1->GetHeight() / 2.f }))
				{
					layer0->GetHit();
					layer1->GetHit();
				}
			}
		}
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
