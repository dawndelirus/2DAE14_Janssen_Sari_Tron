#include "Subject.h"
#include "MiniginPCH.h"
#include "Subject.h"
#include <algorithm>

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	m_Observers.push_back(observer);
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	for (size_t i = 0; i < m_Observers.size(); ++i)
	{
		if (m_Observers[i].lock() == observer)
		{
			m_Observers.erase(m_Observers.begin() + i);
		}
	}
}

void dae::Subject::Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event)
{
	for (size_t i {}; i < m_Observers.size(); ++i)
	{
		if (!m_Observers[i].expired())
		{
			m_Observers[i].lock()->Notify(gameObject, event);
		}
	}
}
