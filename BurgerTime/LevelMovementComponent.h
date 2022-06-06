#pragma once
#include "BaseComponent.h"

class LevelMovementComponent final : public dae::BaseComponent
{
public:
	LevelMovementComponent(std::weak_ptr<dae::GameObject> gameObject);
	~LevelMovementComponent() = default;

	void MoveOnGrid(glm::vec3& position, const glm::vec2& displacement);
	void SetWalkableGrid(std::vector<bool> walkableGrid) { m_IsWalkable = walkableGrid; }
	void SetTileSize(const glm::vec2& size);
	void SetGridWidth(int width) { m_GridWidth = width; }

	void Update() override {};
	void Render() const override {};

private:
	int GetGridIdx(const glm::vec2& pos) const;
	glm::vec2 GetGridTopLeft(size_t idx) const;
	glm::vec2 GetGridBottomCenter(size_t idx) const;
	
	std::vector<bool> m_IsWalkable;
	
	int m_TileWidth;
	int m_TileHeight;

	int m_GridWidth;
};
