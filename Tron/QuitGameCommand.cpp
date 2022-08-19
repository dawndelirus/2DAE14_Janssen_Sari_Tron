#include "QuitGameCommand.h"
#include "ServiceLocator.h"

QuitGameCommand::QuitGameCommand(std::shared_ptr<dae::BaseComponent> component)
	: Command(component)
{
}

void QuitGameCommand::Execute()
{
	dae::ServiceLocator::GetInputManager().Quit();
}
