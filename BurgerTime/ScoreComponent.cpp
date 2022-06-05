#include "BurgerTimePCH.h"
#include "ScoreComponent.h"

void ScoreComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, ObserverEvent event)
{
	switch (event)
	{
	case ObserverEvent::TakeDamage:
		break;
	case ObserverEvent::HasDied:
		break;
	case ObserverEvent::SubjectDeleted:
		break;
	case ObserverEvent::BurgerDropped:
		break;
	default:
		break;
	}


}
