#include "BulletComponent.h"
#include "Clock.h"
#include "LevelLayoutComponent.h"
#include "ObserverHelpers.h"

BulletComponent::BulletComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
    : BaseComponent(gameObject)
    , m_BulletState{}
    , m_Source{}
    , m_LevelLayout{levelLayout}
    , m_BulletSpeed{}
    , m_MaxBounces{0}
    , m_CurrentBounces{-1}
    , m_IsInPool{true}
{
}

void BulletComponent::InitializeBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed, Type bulletSource)
{
    GetGameObject()->SetWorldPosition(glm::vec3(startPos.x, startPos.y, 0.f));
    m_BulletState.velocity = glm::normalize(glm::vec2(direction.x, direction.y));
    m_MaxBounces = bounces;
    m_CurrentBounces = bounces;
    m_Source = bulletSource;
    m_BulletSpeed = bulletSpeed;

    m_IsInPool = false;
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

bool BulletComponent::GetIsInPool() const
{
    return m_IsInPool;
}

void BulletComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
    if (!IsInUse())
    {
        return;
    }
    
    auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event);
    if (observerEvent != nullptr)
    {
        m_CurrentBounces = -1;
    }
}

void BulletComponent::SetIsInPool(bool isInPool)
{
    m_IsInPool = isInPool;
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


    bool isBulletLeft = oldidx - 1 == newIdx || (oldidx - gridWidth) - 1 == newIdx || (oldidx - gridWidth) + 1 == newIdx;
    bool isBulletRight = oldidx + 1 == newIdx || (oldidx + gridWidth) - 1 == newIdx || (oldidx + gridWidth) + 1 == newIdx;


    if (isBulletLeft || isBulletRight)
    {
        float x = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).x + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = x - oldPosition.x;

        x -= abs(leftover);

        newPosition.x = x;
        m_BulletState.velocity.x *= -1;
        --m_CurrentBounces;
    }

    bool isBulletUp = oldidx - gridWidth == newIdx || (oldidx - gridWidth) - 1 == newIdx || (oldidx - gridWidth) + 1 == newIdx;
    bool isBulletDown = oldidx + gridWidth == newIdx || (oldidx + gridWidth) - 1 == newIdx || (oldidx + gridWidth) + 1 == newIdx;

    if (isBulletUp || isBulletDown)
    {
        float y = m_LevelLayout.lock()->GetGridCenterVisuals(newIdx).y + m_LevelLayout.lock()->GetVisualTileWidth() / 2.f;
        float leftover = y - oldPosition.y;
        
        if (isBulletUp)
        {
            y += abs(leftover);
        }
        else
        {
            y -= abs(leftover);
        }

        newPosition.y = y;
        m_BulletState.velocity.y *= -1;
        --m_CurrentBounces;
    }
}