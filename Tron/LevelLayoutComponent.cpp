#include "LevelLayoutComponent.h"
#include <fstream>
#include <sstream>
#include <assert.h>

LevelLayoutComponent::LevelLayoutComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& filePath, int tileWidth, int tileHeight)
    : BaseComponent(gameObject)
    , m_TileWidth{tileWidth}
    , m_TileHeight{tileHeight}
    , m_GridWidth{}
{
    LoadLevel(filePath);
}


void LevelLayoutComponent::LoadLevel(std::string filePath)
{
    m_LevelPath.clear();

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
        while (ss >> number)
        {
            m_LevelPath.emplace_back(number);
            if (ss.peek() == ',')
            {
                ss.ignore();
            }
        }
        ++gridHeight;
    }
    inFile.close();

    m_GridWidth = static_cast<int>(m_LevelPath.size()) / gridHeight;
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
    assert(index < m_LevelPath.size() && "Index is out of bounds in LevelLayoutComponent");
    return m_LevelPath[index];
}

int LevelLayoutComponent::GetTileWidth()
{
    return m_TileWidth;
}

int LevelLayoutComponent::GetTileHeight()
{
    return m_TileHeight;
}

int LevelLayoutComponent::GetGridIndex(const glm::vec2& pos) const
{
    int x = static_cast<int>(pos.x - GetGameObject()->GetWorldPosition().x) / (m_TileWidth);
    int y = static_cast<int>(pos.y - GetGameObject()->GetWorldPosition().y) / (m_TileHeight);

    return y * m_GridWidth + x;
}

glm::vec2 LevelLayoutComponent::GetGridTopLeft(size_t idx) const
{
    glm::vec2 gridPos{};
    gridPos.x = static_cast<float>(static_cast<int>(idx) % m_GridWidth * m_TileWidth);
    gridPos.y = static_cast<float>(static_cast<int>(idx) / m_GridWidth * m_TileHeight);
    return gridPos;
}

glm::vec2 LevelLayoutComponent::GetGridCenter(size_t idx) const
{
    auto gridPos = GetGridTopLeft(idx);
    gridPos.x += m_TileWidth / 2.f;
    gridPos.y += m_TileHeight;
    return gridPos;
}
