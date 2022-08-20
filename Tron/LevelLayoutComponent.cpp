#include "LevelLayoutComponent.h"
#include <fstream>
#include <sstream>
#include <assert.h>

LevelLayoutComponent::LevelLayoutComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& filePath, int tileWidth, int tileHeight)
    : BaseComponent(gameObject)
    , m_LevelTileWidth{tileWidth}
    , m_VisualTileWidth{tileHeight}
    , m_GridWidth{}
    , m_Teleporter{}
{
    LoadLevel(filePath);
}


void LevelLayoutComponent::LoadLevel(std::string filePath)
{
    m_LevelPath.clear();
    m_VisualsPath.clear();

    std::ifstream inFile{};
    inFile.open(filePath);

    int number{};
    std::string line{};

    assert(inFile.is_open() && "Could not open file");

    int gridHeight{};
    while (!inFile.eof())
    {
        std::getline(inFile, line);
        std::stringstream ss(line);
        int i{};
        while (ss >> number)
        {
            if (gridHeight % 2 == 0 && i % 2 == 0)
            {
                m_LevelPath.emplace_back(number > 1);
                if (m_LevelPath.size() > 0)
                {
				    SaveObjectPositions(number, static_cast<int>(m_LevelPath.size() - 1));
                }
            }

            m_VisualsPath.emplace_back(number);
            if (ss.peek() == ',')
            {
                ss.ignore();
            }
            ++i;
        }
        ++gridHeight;
    }
    inFile.close();
    gridHeight /= 2;

    m_GridWidth = static_cast<int>(m_LevelPath.size()) / gridHeight;
}

void LevelLayoutComponent::SaveObjectPositions(int number, int index)
{
    switch (number)
    {
    case static_cast<int>(Objects::Player):
        m_PlayerPositions.emplace_back(index);
        break;
    case static_cast<int>(Objects::TankEnemy):
        m_EnemyTankPositions.emplace_back(index);
        break;
    case static_cast<int>(Objects::RecognizerEnemy):
        m_EnemyRecognizerPositions.emplace_back(index);
        break;
    default:
        break;
    }

    if (number == 3)
    {
        m_Teleporter = index;
    }
}

int LevelLayoutComponent::GetGridWidth()
{
    return m_GridWidth;
}

int LevelLayoutComponent::GetGridSize()
{
    return static_cast<int>(m_LevelPath.size());
}

bool LevelLayoutComponent::IsWalkable(int index)
{
    assert(static_cast<size_t>(index) < m_LevelPath.size() && "Index is out of bounds in LevelLayoutComponent");
    return m_LevelPath[index];
}

int LevelLayoutComponent::GetLevelTileWidth()
{
    return m_LevelTileWidth;
}

int LevelLayoutComponent::GetRandomWalkableGridIdx()
{
    int teleportAttempts{ 20 };
    for (size_t i{}; i < teleportAttempts; ++i)
    {
        int randomIdx = rand() % static_cast<int>(m_LevelPath.size());
        if (IsWalkable(randomIdx))
        {
            return randomIdx;
        }
    }
    return m_Teleporter;
}

int LevelLayoutComponent::GetTeleportIdx() const
{
    return m_Teleporter;
}

const std::vector<int>& LevelLayoutComponent::GetVisualsVector()
{
    return m_VisualsPath;
}

bool LevelLayoutComponent::IsOnVisualPath(int index)
{
    if (static_cast<size_t>(index) > m_VisualsPath.size() || index < 0)
    {
        return false;
    }
    return m_VisualsPath[index];
}

int LevelLayoutComponent::GetVisualsGridSize()
{
    return static_cast<int>(m_VisualsPath.size());
}

int LevelLayoutComponent::GetVisualTileWidth()
{
    return m_VisualTileWidth;
}

int LevelLayoutComponent::GetVisualGridIndex(const glm::vec2& pos)
{
    int x = static_cast<int>(pos.x - GetGameObject()->GetWorldPosition().x) / m_VisualTileWidth;
    int y = static_cast<int>(pos.y - GetGameObject()->GetWorldPosition().y) / m_VisualTileWidth;

    return y * (m_GridWidth * 2) + x;
}

glm::vec2 LevelLayoutComponent::GetGridCenterVisuals(int idx)
{
    int tileScale = m_LevelTileWidth / m_VisualTileWidth;

    glm::vec2 gridPos{};
    gridPos.x = static_cast<float>(static_cast<int>(idx) % (m_GridWidth * tileScale) * m_VisualTileWidth);
    gridPos.y = static_cast<float>(static_cast<int>(idx) / (m_GridWidth * tileScale) * m_VisualTileWidth);

    gridPos.x += m_VisualTileWidth / 2.f + GetGameObject()->GetLocalPosition().x;
    gridPos.y += m_VisualTileWidth / 2.f + GetGameObject()->GetLocalPosition().y;
    return gridPos;
}

int LevelLayoutComponent::GetGridIndex(const glm::vec2& pos) const
{
    int x = static_cast<int>(pos.x - GetGameObject()->GetWorldPosition().x) / (m_LevelTileWidth);
    int y = static_cast<int>(pos.y - GetGameObject()->GetWorldPosition().y) / (m_LevelTileWidth);

    return y * m_GridWidth + x;
}

glm::vec2 LevelLayoutComponent::GetGridTopLeft(size_t idx) const
{
    glm::vec2 gridPos{};
    gridPos.x = (static_cast<float>(static_cast<int>(idx) % m_GridWidth) * m_LevelTileWidth);
    gridPos.y = (static_cast<float>(static_cast<int>(idx) / m_GridWidth) * m_LevelTileWidth);
    return gridPos;
}

glm::vec2 LevelLayoutComponent::GetGridCenter(size_t idx) const
{
    auto gridPos = GetGridTopLeft(idx);
    gridPos.x += (m_LevelTileWidth / 2.f) + GetGameObject()->GetLocalPosition().x;
    gridPos.y += (m_LevelTileWidth / 2.f) + GetGameObject()->GetLocalPosition().y;
    return gridPos;
}

std::vector<int> LevelLayoutComponent::GetConnectingIndices(int idx)
{
    std::vector<int> connectedIdx{};
    int index = idx + 1;
    if (IsWalkable(index))
    {
        connectedIdx.push_back(index);
    }
    index = idx - 1;
    if (IsWalkable(index))
    {
        connectedIdx.push_back(index);
    }
    index = idx - m_GridWidth;
	if (IsWalkable(index))
	{
		connectedIdx.push_back(index);
	}
    index = idx + m_GridWidth;
    if (IsWalkable(index))
    {
        connectedIdx.push_back(index);
    }

    return connectedIdx;
}

const std::vector<int>& LevelLayoutComponent::GetPlayerStartPositions() const
{
    return m_PlayerPositions;
}

const std::vector<int>& LevelLayoutComponent::GetEnemyTankStartPositions() const
{
    return m_EnemyTankPositions;
}

const std::vector<int>& LevelLayoutComponent::GetEnemyRecognizerStartPositions() const
{
    return m_EnemyRecognizerPositions;
}
