#include "HealthDisplayComponent.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ObserverHelpers.h"
#include "ObserverEvents.h"

HealthDisplayComponent::HealthDisplayComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void HealthDisplayComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	auto observerEvent = std::dynamic_pointer_cast<HealthChangedObserverEvent>(event);
	if (observerEvent != nullptr)
	{
		Subject::Notify(nullptr, std::make_shared<dae::TextChangedObserverEvent>("Remaining lives: " + std::to_string(observerEvent->currentHp)));
	}
}
