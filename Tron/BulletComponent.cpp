#include "BulletComponent.h"
#include "Clock.h"
#include "LevelLayoutComponent.h"

BulletComponent::BulletComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
    : BaseComponent(gameObject)
    , m_BulletState{}
    , m_Source{}
    , m_LevelLayout{levelLayout}
    , m_BulletSpeed{}
    , m_MaxBounces{0}
    , m_CurrentBounces{-1}
{
}

void BulletComponent::InitializeBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed, Type bulletSource)
{
    GetGameObject()->SetWorldPosition(glm::vec3(startPos.x, startPos.y, 0.f));
    m_BulletState.velocity = direction;
    m_MaxBounces = bounces;
    m_CurrentBounces = bounces;
    m_Source = bulletSource;
    m_BulletSpeed = bulletSpeed;
}

void BulletComponent::SetNext(BulletComponent* next)
{
    m_BulletState.next = next;
}

BulletComponent* BulletComponent::GetNext()
{
    return m_BulletState.next;
}

bool BulletComponent::IsInUse()
{
    return m_CurrentBounces >= 0;
}

void BulletComponent::Update()
{
    if (!IsInUse())
    {
        return;
    }

    auto pos = GetGameObject()->GetWorldPosition();
    auto newPos = pos;
    newPos.x += (m_BulletState.velocity.x * m_BulletSpeed * dae::Clock::GetDeltaTime());
    newPos.y += (m_BulletState.velocity.y * m_BulletSpeed * dae::Clock::GetDeltaTime());
    Bounce(pos, newPos);
    GetGameObject()->SetWorldPosition(newPos);
}

void BulletComponent::Bounce(const glm::vec3& oldPosition, glm::vec3& newPosition)
{
    int oldidx = m_LevelLayout.lock()->GetVisualGridIndex(oldPosition);
    int newIdx = m_LevelLayout.lock()->GetVisualGridIndex(newPosition);

    if (oldidx == newIdx || m_LevelLayout.lock()->IsOnVisualPath(newIdx))
    {
        return;
    }

    float gridWidth = m_LevelLayout.lock()->GetGridWidth() * 2.f;
    if (oldidx - 1 == newIdx || (oldidx - gridWidth) - 1 == newIdx || (oldidx - gridWidth) + 1 == newIdx)
    {
        float x = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).x + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = x - oldPosition.x;

        x -= abs(leftover);

        newPosition.x = x;
        m_BulletState.velocity.x *= -1;
        --m_CurrentBounces;
    }
    else if (oldidx + 1 == newIdx || (oldidx + gridWidth) - 1 == newIdx || (oldidx + gridWidth) + 1 == newIdx)
    {
        float x = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).x + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = x - oldPosition.x;

        x += abs(leftover);;

        newPosition.x = x;
        m_BulletState.velocity.x *= -1;
        --m_CurrentBounces;
    }

    if (oldidx - gridWidth == newIdx || (oldidx - gridWidth) - 1 == newIdx || (oldidx - gridWidth) + 1 == newIdx)
    {
        float y = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).y + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = y - oldPosition.y;

        y += abs(leftover);;

        newPosition.y = y;
        m_BulletState.velocity.y *= -1;
        --m_CurrentBounces;
    }
    else if (oldidx + gridWidth == newIdx || (oldidx + gridWidth) - 1 == newIdx || (oldidx + gridWidth) + 1 == newIdx)
    {
        float y = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).y + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = y - oldPosition.y;

        y -= abs(leftover);;

        newPosition.y = y;
        m_BulletState.velocity.y *= -1;
        --m_CurrentBounces;
    }
}