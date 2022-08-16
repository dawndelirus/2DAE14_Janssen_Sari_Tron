#pragma once
#include "BaseObserverEvent.h"
#include <string>

namespace dae
{
	struct SubjectDeletedObserverEvent final : public BaseObserverEvent
	{
		SubjectDeletedObserverEvent() = default;
	};

	struct TextChangedObserverEvent final : public BaseObserverEvent
	{
		TextChangedObserverEvent(std::string text)
			: text(text)
		{}

		std::string text;
	};
}