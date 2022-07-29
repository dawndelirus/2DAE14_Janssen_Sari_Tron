#include "MiniginPCH.h"
#include "Subject.h"
#include <algorithm>

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	m_Observers.push_back(observer);
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
	if (it != m_Observers.end())
	{
		m_Observers.erase(it);
	}
}

void dae::Subject::Notify(std::shared_ptr<GameObject> gameObject, ObserverEvent event)
{
	for (size_t i {}; i < m_Observers.size(); ++i)
	{
		m_Observers[i]->Notify(gameObject, event);
	}
}
