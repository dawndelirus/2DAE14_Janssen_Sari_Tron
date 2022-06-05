#pragma once
#include "BaseComponent.h"
#include "unordered_map"

class LevelLoaderComponent final : public dae::BaseComponent
{
public:
	enum class PlayerIndex
	{
		pepper = 5,
		salt = 6,
		hotdog = 7
	};

	enum class EnemyIndex
	{
		hotdog = 5,
		pickle = 6,
		egg = 7
	};

	LevelLoaderComponent(std::weak_ptr<dae::GameObject> gameObject, int gridWidth);
	~LevelLoaderComponent() = default;

	void Update() override {};
	void Render() const override {};

	void LoadLevel(const std::string& levelPath);

	const std::unordered_map<int, glm::vec2>& GetPlayerPositions() const;
	const std::unordered_multimap<int, glm::vec2>& GetEnemyPositions() const { return m_EnemyPositions; }

private:
	glm::vec2 GetGridTopLeft(size_t idx);
	glm::vec2 GetGridBottomCenter(size_t idx);

	void LoadLevelVisuals(std::vector<char> output);
	void LoadLevelFood(std::vector<char> output);
	void LoadPlayerPositions(std::vector<char> output);
	void LoadEnemyPositions(std::vector<char> output);

	std::unordered_map<int, glm::vec2> m_PlayerPositions;
	std::unordered_multimap<int, glm::vec2> m_EnemyPositions;

	int m_TileWidth;
	int m_TileHeight;

	int m_GridWidth;
};
