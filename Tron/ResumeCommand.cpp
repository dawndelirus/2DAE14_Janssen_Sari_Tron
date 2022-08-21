#include "ResumeCommand.h"
#include "LevelManager.h"

ResumeCommand::ResumeCommand(std::shared_ptr<dae::BaseComponent> component)
	: Command(component)
{
}

void ResumeCommand::Execute(float )
{
	LevelManager::GetInstance().HandleInput(LevelManager::InputStates::Resume);
}
