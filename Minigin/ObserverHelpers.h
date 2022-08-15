#pragma once
#include "BaseObserverEvent.h"

struct GetHitObserverEvent : public dae::BaseObserverEvent
{
	GetHitObserverEvent(int amount)
		: amount{ amount } 
	{}
	GetHitObserverEvent()
		: amount{ 1 } 
	{}
	int amount = 1;
};
