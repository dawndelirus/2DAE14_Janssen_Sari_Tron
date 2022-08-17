#include "ScoreComponent.h"
#include "ObserverHelpers.h"
#include "ObserverHelpers.h"

ScoreComponent::ScoreComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_Points{}
{
}

int ScoreComponent::GetScore() const
{
	return m_Points;
}

void ScoreComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<EnemyKilledObserverEvent>(event); observerEvent != nullptr)
	{
		m_Points += observerEvent->points;
		Subject::Notify(nullptr, std::make_shared<ScoreChangedObserverEvent>(m_Points));
	}
}
