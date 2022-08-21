#include "QuitGameCommand.h"
#include "ServiceLocator.h"

QuitGameCommand::QuitGameCommand(std::shared_ptr<dae::BaseComponent> component)
	: Command(component)
{
}

void QuitGameCommand::Execute(float )
{
	dae::ServiceLocator::GetInputManager().Quit();
}
