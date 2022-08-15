#pragma once
#include "BaseObserverEvent.h"

struct GetHitObserverEvent final : public dae::BaseObserverEvent
{
	GetHitObserverEvent(int amount)
		: amount{ amount } 
	{}
	GetHitObserverEvent()
		: amount{ 1 } 
	{}
	int amount = 1;
};

struct TakeDamageObserverEvent final : public dae::BaseObserverEvent
{
	TakeDamageObserverEvent(int currentHp)
		: currentHp{ currentHp }
	{}
	int currentHp = 0;
};

struct DiedObserverEvent final : public dae::BaseObserverEvent
{
	DiedObserverEvent() = default;
};
