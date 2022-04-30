#include "BurgerTimePCH.h"
#include "TakeDamageCommand.h"
#include "HealthComponent.h"

TakeDamageCommand::TakeDamageCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void TakeDamageCommand::Execute()
{
	GetGameObject()->GetComponent<HealthComponent>()->TakeDamage(1);
}
