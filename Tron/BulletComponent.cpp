#include "BulletComponent.h"
#include "Clock.h"

BulletComponent::BulletComponent(std::shared_ptr<dae::GameObject> gameObject)
    : BaseComponent(gameObject)
    , m_BulletState{}
    , m_Source{}
    , m_MaxBounces{}
    , m_CurrentBounces{}
{
}

void BulletComponent::InitializeBullet(glm::vec2 startPos, glm::vec2 direction, int bounces, Source bulletSource)
{
    GetGameObject()->SetWorldPosition(glm::vec3(startPos.x, startPos.y, 0.f));
    m_BulletState.velocity = direction;
    m_MaxBounces = bounces;
    m_CurrentBounces = 0;
    m_Source = bulletSource;
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
    GetGameObject()->SetWorldPosition(glm::vec3(pos.x + m_BulletState.velocity.x + dae::Clock::GetDeltaTime(), pos.y + m_BulletState.velocity.y + dae::Clock::GetDeltaTime(), 0.f));
}
