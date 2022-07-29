#include "LevelMovementComponent.h"

LevelMovementComponent::LevelMovementComponent(dae::GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_GridWidth{}
	, m_IsWalkable{}
	, m_TileHeight{}
	, m_TileWidth{}
{
}

void LevelMovementComponent::MoveOnGrid(glm::vec3& position, const glm::vec2& displacement)
{
	int index = GetGridIdx(position);

	if (displacement.x > 0.f)
	{
		assert((static_cast<size_t>(index) == m_IsWalkable.size() - 1) && "Player is on the edge of the grid");

		//if (index != m_IsWalkable.size() - 2 && (index + 1) % m_GridWidth != 0 && m_IsWalkable[index + 1])
		if (m_IsWalkable[index + 1])
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.x < 0.f)
	{
		assert((index == 0) && "Player is on the edge of the grid");

		//if ((index - 1) >= 0 && (index % m_GridWidth) != 0 && m_IsWalkable[index - 1])
		if (m_IsWalkable[index - 1])
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.y < 0.f)
	{
		assert((index <= m_GridWidth) && "Player is on the edge of the grid");

		//if (index >= m_GridWidth && m_IsWalkable[index - m_GridWidth])
		if (m_IsWalkable[index - m_GridWidth])
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
	else if (displacement.y > 0.f)
	{
		assert((static_cast<size_t>(index + m_GridWidth) >= m_IsWalkable.size()) && "Player is on the edge of the grid");
		
		if (m_IsWalkable[index + m_GridWidth])
		{
			position = (position + glm::vec3(displacement.x, displacement.y, 0.f));
		}
	}
}

void LevelMovementComponent::SetTileSize(const glm::vec2& size)
{
	m_TileWidth = static_cast<int>(size.x);
	m_TileHeight = static_cast<int>(size.y);
}

int LevelMovementComponent::GetGridIdx(const glm::vec2& pos) const
{
	// grid offset
	//int x = static_cast<int>(pos.x) / (m_GridWidth * m_TileWidth);
	//int y = static_cast<int>(pos.y) % (m_GridWidth * m_TileWidth);

	//return x * m_GridWidth + y;
	int x = static_cast<int>(pos.x - GetGameObject()->GetWorldPosition().x) / (m_TileWidth);
	int y = static_cast<int>(pos.y - GetGameObject()->GetWorldPosition().y) / (m_TileHeight);

	return y * m_GridWidth + x;
}

glm::vec2 LevelMovementComponent::GetGridTopLeft(size_t idx) const
{
	glm::vec2 gridPos{};
	gridPos.x = static_cast<float>(static_cast<int>(idx) % m_GridWidth * m_TileWidth);
	gridPos.y = static_cast<float>(static_cast<int>(idx) / m_GridWidth * m_TileHeight);
	return gridPos;
}

glm::vec2 LevelMovementComponent::GetGridBottomCenter(size_t idx) const
{
	auto pos = GetGridTopLeft(idx);
	pos.x += m_TileWidth / 2.f;
	pos.y += m_TileHeight;
	return pos;
}
