#include "PauseCommand.h"
#include "LevelManager.h"

PauseCommand::PauseCommand(std::shared_ptr<dae::BaseComponent> component)
	: Command(component)
{
}

void PauseCommand::Execute()
{
	LevelManager::GetInstance().HandleInput(LevelManager::InputStates::Pause);
}
