#include "LevelMovementComponent.h"
#include "LevelLayoutComponent.h"

LevelMovementComponent::LevelMovementComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_LevelLayout{levelLayout}
{
}

void LevelMovementComponent::MoveOnGrid(glm::vec3& position, const glm::vec2& displacement)
{
	auto levelLayout = m_LevelLayout.lock();
	int index = levelLayout->GetGridIndex(position);

	if (displacement.x > 0.f)
	{
		assert((static_cast<size_t>(index) < levelLayout->GetGridSize() - 1) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index + 1))
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.x < 0.f)
	{
		assert((index > 0) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index - 1))
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.y < 0.f)
	{
		assert((index > m_GridWidth) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index - levelLayout->GetGridWidth()))
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.y > 0.f)
	{
		assert((static_cast<size_t>(index + levelLayout->GetGridWidth()) < levelLayout->GetGridSize()) && "Player is on the edge of the grid");
		
		if (levelLayout->IsWalkable(index + levelLayout->GetGridWidth()))
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
}
