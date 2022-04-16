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

		virtual void AddObserver(std::shared_ptr<Observer> observer);
		virtual void RemoveObserver(std::shared_ptr<Observer> observer);

	protected:
		virtual void Notify(std::shared_ptr<GameObject> gameObject, Event event);

	private:
		std::vector<std::shared_ptr<Observer>> m_Observers;
	};
}
