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
	auto help = levelLayout->GetGridCenter(index);
	help;
	if (displacement.x > 0.f)
	{
		assert((static_cast<size_t>(index) < levelLayout->GetGridSize() - 1) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index + 1))
		{
			float gridCenterY = levelLayout->GetGridCenter(index).y;

			if (position.y - gridCenterY > 0.01f)
			{
				position -= glm::vec3(displacement.y, displacement.x, 0.f);
				
				if (position.y - gridCenterY < 0.f)
				{
					position.y = gridCenterY;
				}
			}
			else if (position.y - gridCenterY < -0.01f)
			{
				position += glm::vec3(displacement.y, displacement.x, 0.f);
				
				if (position.y - gridCenterY > 0.f)
				{
					position.y = gridCenterY;
				}
			}
			else
			{
				position += glm::vec3(displacement.x, displacement.y, 0.f);
				position.y = gridCenterY;
			}
		}
	}
	else if (displacement.x < 0.f)
	{
		assert((index > 0) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index - 1))
		{
			float gridCenterY = levelLayout->GetGridCenter(index).y;

			if (position.y - gridCenterY > 0.01f)
			{
				position += glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.y - gridCenterY < 0.f)
				{
					position.y = gridCenterY;
				}
			}
			else if (position.y - gridCenterY < -0.01f)
			{
				position -= glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.y - gridCenterY > 0.f)
				{
					position.y = gridCenterY;
				}
			}
			else
			{
				position += glm::vec3(displacement.x, displacement.y, 0.f);
				position.y = gridCenterY;
			}
		}
	}
	else if (displacement.y < 0.f)
	{
		assert((index > levelLayout->GetGridWidth()) && "Player is on the edge of the grid");

		if (levelLayout->IsWalkable(index - levelLayout->GetGridWidth()))
		{
			float gridCenterX = levelLayout->GetGridCenter(index).x;

			if (position.x - gridCenterX > 0.01f)
			{
				position += glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.x - gridCenterX < 0.f)
				{
					position.x = gridCenterX;
				}
			}
			else if (position.x - gridCenterX < -0.01f)
			{
				position -= glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.x - gridCenterX > 0.f)
				{
					position.x = gridCenterX;
				}
			}
			else
			{
				position += glm::vec3(displacement.x, displacement.y, 0.f);
				position.x = gridCenterX;
			}
		}
	}
	else if (displacement.y > 0.f)
	{
		assert((static_cast<size_t>(index + levelLayout->GetGridWidth()) < levelLayout->GetGridSize()) && "Player is on the edge of the grid");
		
		if (levelLayout->IsWalkable(index + levelLayout->GetGridWidth()))
		{
			float gridCenterX = levelLayout->GetGridCenter(index).x;

			if (position.x - gridCenterX > 0.01f)
			{
				position -= glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.x - gridCenterX < 0.f)
				{
					position.x = gridCenterX;
				}
			}
			else if (position.x - gridCenterX < -0.01f)
			{
				position += glm::vec3(displacement.y, displacement.x, 0.f);

				if (position.x - gridCenterX > 0.f)
				{
					position.x = gridCenterX;
				}
			}
			else
			{
				position += glm::vec3(displacement.x, displacement.y, 0.f);
				position.x = gridCenterX;
			}
		}
	}
}
