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
	
	glm::vec2 gridCenter = levelLayout->GetGridCenter(index);

	if (abs(displacement.x) > 0.f)
	{
		if (displacement.x > 0.f)
		{
			assert((static_cast<size_t>(index) < levelLayout->GetGridSize() - 1) && "Player is on the edge of the grid");
			++index;
		}
		else if (displacement.x < 0.f)
		{
			assert((index > 0) && "Player is on the edge of the grid");
			--index;
		}

		if (!levelLayout->IsWalkable(index))
		{
			// If next grid is non walkable, move to center of current grid
			if (IsLeftGridCenter(position, gridCenter))
			{
				position -= glm::vec3(abs(displacement.x), 0.f, 0.f);
				if (IsRightGridCenter(position, gridCenter))
				{
					position.x = gridCenter.x;
				}
				return;
			}
			else if (IsRightGridCenter(position, gridCenter))
			{
				position += glm::vec3(abs(displacement.x), 0.f, 0.f);
				if (IsLeftGridCenter(position, gridCenter))
				{
					position.x = gridCenter.x;
				}
			}
			return;
		}

		if (IsBelowGridCenter(position, gridCenter))
		{
			position -= glm::vec3(0.f, abs(displacement.x), 0.f);
			if (IsAboveGridCenter(position, gridCenter))
			{
				position.y = gridCenter.y;
			}
		}
		else if (IsAboveGridCenter(position, gridCenter))
		{
			position += glm::vec3(0.f, abs(displacement.x), 0.f);
			if (IsBelowGridCenter(position, gridCenter))
			{
				position.y = gridCenter.y;
			}
		}
		else
		{
			position += glm::vec3(displacement.x, 0.f, 0.f);
			position.y = gridCenter.y;
		}
	}
	else if (abs(displacement.y) > 0.f)
	{
		if (displacement.y < 0.f)
		{
			assert((index > levelLayout->GetGridWidth()) && "Player is on the edge of the grid");
			index -= levelLayout->GetGridWidth();
		}
		else if (displacement.y > 0.f)
		{
			assert((static_cast<size_t>(index + levelLayout->GetGridWidth()) < levelLayout->GetGridSize()) && "Player is on the edge of the grid");
			index += levelLayout->GetGridWidth();
		}

		if (!levelLayout->IsWalkable(index))
		{
			// If next grid is non walkable, move to center of current grid
			if (IsBelowGridCenter(position, gridCenter))
			{
				position -= glm::vec3(0.f, abs(displacement.y), 0.f);
				if (IsAboveGridCenter(position, gridCenter))
				{
					position.y = gridCenter.y;
				}
			}
			else if (IsAboveGridCenter(position, gridCenter))
			{
				position += glm::vec3(0.f, abs(displacement.y), 0.f);
				if (IsBelowGridCenter(position, gridCenter))
				{
					position.y = gridCenter.y;
				}
			}
			
			return;
		}

		if (IsLeftGridCenter(position, gridCenter))
		{
			position -= glm::vec3(abs(displacement.y), 0.f, 0.f);
			if (IsRightGridCenter(position, gridCenter))
			{
				position.x = gridCenter.x;
			}
		}
		else if (IsRightGridCenter(position, gridCenter))
		{
			position += glm::vec3(abs(displacement.y), 0.f, 0.f);
			if (IsLeftGridCenter(position, gridCenter))
			{
				position.x = gridCenter.x;
			}
		}
		else
		{
			position += glm::vec3(0.f, displacement.y, 0.f);
			position.x = gridCenter.x;
		}
	}
}

bool LevelMovementComponent::IsBelowGridCenter(const glm::vec2& position, const glm::vec2& gridCenter)
{
	return (position.y - gridCenter.y > 0.01f);
}

bool LevelMovementComponent::IsAboveGridCenter(const glm::vec2& position, const glm::vec2& gridCenter)
{
	return (position.y - gridCenter.y < -0.01f);
}

bool LevelMovementComponent::IsLeftGridCenter(const glm::vec2& position, const glm::vec2& gridCenter)
{
	return (position.x - gridCenter.x > 0.01f);
}

bool LevelMovementComponent::IsRightGridCenter(const glm::vec2& position, const glm::vec2& gridCenter)
{
	return (position.x - gridCenter.x < -0.01f);
}
