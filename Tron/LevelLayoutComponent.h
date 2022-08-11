#pragma once
#include "BaseComponent.h"
#include <string>
#include <vector>

class LevelLayoutComponent final : public dae::BaseComponent
{
public:
	LevelLayoutComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& filePath, int levelTileWidth, int gridTileWidth);
	~LevelLayoutComponent() = default;

	bool IsWalkable(int index);
	int GetGridSize();
	int GetGridWidth();
	int GetLevelTileWidth();

	const std::vector<int>& GetVisualsVector();
	bool IsOnVisualPath(int index);
	int GetVisualsGridSize();
	int GetVisualTileWidth();
	int GetVisualGridIndex(const glm::vec2& pos);
	glm::vec2 GetGridCenterVisuals(int idx);

	int GetGridIndex(const glm::vec2& pos) const;
	glm::vec2 GetGridTopLeft(size_t idx) const;
	glm::vec2 GetGridCenter(size_t idx) const;
	std::vector<int> GetConnectingIndices(int idx);

	const std::vector<int>& GetPlayerStartPositions() const;
	const std::vector<int>& GetEnemyStartPositions() const;

	void Update() override {}
	void Render() const override {}

private:
	enum class Objects
	{
		Player = 3,
		Enemy = 4
	};

	void LoadLevel(std::string filePath);
	void SaveObjectPositions(int number, int index);

	std::vector<bool> m_LevelPath;
	std::vector<int> m_VisualsPath;
	std::vector<int> m_PlayerPositions;
	std::vector<int> m_EnemyPositions;

	int m_LevelTileWidth;
	int m_VisualTileWidth;
	int m_GridWidth;
};

