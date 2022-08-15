#pragma once
#include "BaseObserverEvent.h"

namespace dae
{
	struct SubjectDeletedEvent final : public BaseObserverEvent
	{
		SubjectDeletedEvent() = default;
	};
}