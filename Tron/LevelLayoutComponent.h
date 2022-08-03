#pragma once
#include "BaseComponent.h"
#include <string>
#include <vector>

class LevelLayoutComponent final : public dae::BaseComponent
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

	const std::vector<int>& GetPlayerStartPositions() const;
	const std::vector<int>& GetEnemyStartPositions() const;

	void Update() override {}
	void Render() const override {}

private:
	enum class Objects
	{
		Player = 2,
		Enemy = 3
	};

	void LoadLevel(std::string filePath);
	void SaveObjectPositions(int number, int index);

	std::vector<bool> m_LevelPath;
	std::vector<int> m_PlayerPositions;
	std::vector<int> m_EnemyPositions;

	int m_TileWidth;
	int m_TileHeight;
	int m_GridWidth;

};

