#pragma once
#include "Observer.h"
#include <vector>
#include <memory>

namespace dae
{
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(std::shared_ptr<Observer> observer);
		void RemoveObserver(std::shared_ptr<Observer> observer);

	protected:
		void Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event);

	private:
		std::vector<std::weak_ptr<Observer>> m_Observers;
	};
}
