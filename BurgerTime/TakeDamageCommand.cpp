#include "BurgerTimePCH.h"
#include "TakeDamageCommand.h"
#include "HealthComponent.h"

TakeDamageCommand::TakeDamageCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void TakeDamageCommand::Execute()
{
	// TODO: COMMAND: save as component?
	GetGameObject()->GetComponent<HealthComponent>()->TakeDamage(1);
}
