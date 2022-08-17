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
	if (auto observerEvent = std::dynamic_pointer_cast<HealthChangedObserverEvent>(event); observerEvent != nullptr)
	{
		Subject::Notify(nullptr, std::make_shared<dae::TextChangedObserverEvent>("Lives: " + std::to_string(observerEvent->currentHp)));
	}
}
