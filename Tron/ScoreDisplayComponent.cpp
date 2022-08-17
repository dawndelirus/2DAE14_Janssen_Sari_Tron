#include "ScoreDisplayComponent.h"
#include "ObserverHelpers.h"
#include "ObserverEvents.h"

ScoreDisplayComponent::ScoreDisplayComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void ScoreDisplayComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<ScoreChangedObserverEvent>(event); observerEvent != nullptr)
	{
		Subject::Notify(nullptr, std::make_shared<dae::TextChangedObserverEvent>("Score: " + std::to_string(observerEvent->currentScore)));
	}
}
