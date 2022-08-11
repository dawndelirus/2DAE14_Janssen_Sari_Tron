#include "BulletComponent.h"
#include "Clock.h"

BulletComponent::BulletComponent(std::shared_ptr<dae::GameObject> gameObject)
    : BaseComponent(gameObject)
    , m_BulletState{}
    , m_Source{}
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
    return m_CurrentBounces >= m_MaxBounces;
}

void BulletComponent::Update()
{
    if (!IsInUse())
    {
        return;
    }

    auto pos = GetGameObject()->GetWorldPosition();
    pos.x += (m_BulletState.velocity.x * m_BulletSpeed * dae::Clock::GetDeltaTime());
    pos.y += (m_BulletState.velocity.y * m_BulletSpeed * dae::Clock::GetDeltaTime());
    GetGameObject()->SetWorldPosition(pos);
}
