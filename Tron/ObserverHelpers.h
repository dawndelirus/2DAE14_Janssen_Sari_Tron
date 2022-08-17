#pragma once
#include "BaseObserverEvent.h"
#include "CollisionHandlerComponent.h"

struct GetHitObserverEvent final : public dae::BaseObserverEvent
{
	GetHitObserverEvent(CollisionHandlerComponent::Layer layer)
		: layer(layer)
	{}
	CollisionHandlerComponent::Layer layer;
};

struct TakeDamageObserverEvent final : public dae::BaseObserverEvent
{
	TakeDamageObserverEvent(int amount)
		: amount{ amount }
	{}
	int amount = 0;
};

struct HealthChangedObserverEvent final : public dae::BaseObserverEvent
{
	HealthChangedObserverEvent(int currentHp)
		: currentHp{ currentHp }
	{}
	int currentHp = 0;
};

struct ScoreChangedObserverEvent final : public dae::BaseObserverEvent
{
	ScoreChangedObserverEvent(int currentScore)
		: currentScore{ currentScore }
	{}
	int currentScore = 0;
};

struct DiedObserverEvent final : public dae::BaseObserverEvent
{
	DiedObserverEvent() = default;
};

struct EnemyKilledObserverEvent final : public dae::BaseObserverEvent
{
	EnemyKilledObserverEvent(int points)
		: points{ points }
	{}
	int points = 0;
};
