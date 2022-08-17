#include "Subject.h"
#include "MiniginPCH.h"
#include "Subject.h"
#include <algorithm>

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	m_Observers.push_back(observer.get());
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	auto it = std::find(m_Observers.begin(), m_Observers.end(), observer.get());
	if (it != m_Observers.end())
	{
		m_Observers.erase(it);
	}
}

void dae::Subject::Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event)
{
	for (size_t i {}; i < m_Observers.size(); ++i)
	{
		m_Observers[i]->Notify(gameObject, event);
	}
}
