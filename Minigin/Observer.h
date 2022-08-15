#pragma once
#include "BaseObserverEvent.h"
#include "GameObject.h"
#include <memory>

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event) = 0;
	};
}