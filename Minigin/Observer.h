#pragma once
#include "ObserverHelpers.h"
#include "GameObject.h"
#include <memory>

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(std::shared_ptr<GameObject> gameObject, Event event) = 0;
	};
}