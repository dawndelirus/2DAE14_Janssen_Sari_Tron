#include "BurgerTimePCH.h"
#include "SprayCommand.h"
#include "SprayComponent.h"

SprayCommand::SprayCommand(std::shared_ptr<dae::GameObject> pGameObject)
	: Command{ pGameObject }
{
}

void SprayCommand::Execute()
{
	GetGameObject()->GetComponent<SprayComponent>()->SprayPepper();
}
