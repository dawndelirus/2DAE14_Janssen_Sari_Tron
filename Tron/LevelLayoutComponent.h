#pragma once
#include "BaseComponent.h"
#include <string>
#include <vector>

class LevelLayoutComponent final : dae::BaseComponent
{
public:
	LevelLayoutComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& filePath, int tileWidth, int tileHeight);
	~LevelLayoutComponent() = default;

	bool IsWalkable(int index);
	int GetGridSize();
	int GetGridWidth();
	int GetTileWidth();
	int GetTileHeight();

	int GetGridIndex(const glm::vec2& pos) const;
	glm::vec2 GetGridTopLeft(size_t idx) const;
	glm::vec2 GetGridCenter(size_t idx) const;

	void Update() override {}
	void Render() const override {}

private:
	void LoadLevel(std::string filePath);
	
	std::vector<bool> m_LevelPath;
	int m_TileWidth;
	int m_TileHeight;
	int m_GridWidth;

};

