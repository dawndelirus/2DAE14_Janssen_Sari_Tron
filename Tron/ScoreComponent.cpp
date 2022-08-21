#include "ScoreComponent.h"
#include "ObserverHelpers.h"
#include "ObserverHelpers.h"
#include "GameInfo.h"	

ScoreComponent::ScoreComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_Points{}
	, m_AssignPoints{false}
{
}

int ScoreComponent::GetScore() const
{
	return m_Points;
}

void ScoreComponent::SetScore(int score)
{
	m_Points = score;
}

void ScoreComponent::Update()
{
	if (m_AssignPoints)
	{
		GameInfo::GetInstance().SetPlayerScore(m_Points);
	}
}

void ScoreComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<EnemyKilledObserverEvent>(event); observerEvent != nullptr)
	{
		m_Points += observerEvent->points;
		Subject::Notify(nullptr, std::make_shared<ScoreChangedObserverEvent>(m_Points));
		GameInfo::GetInstance().SetPlayerScore(m_Points);
	}
	if (auto observerEvent = std::dynamic_pointer_cast<EnemiesDeadObserverEvent>(event); observerEvent != nullptr)
	{
		m_AssignPoints = true;
	}
	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		m_AssignPoints = true;
	}
}
