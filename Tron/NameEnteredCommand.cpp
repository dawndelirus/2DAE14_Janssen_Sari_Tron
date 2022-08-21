#include "NameEnteredCommand.h"
#include "HighScoreComponent.h"

NameEnteredCommand::NameEnteredCommand(std::shared_ptr<HighScoreComponent> component)
	: Command(component)
{
}

void NameEnteredCommand::Execute(float)
{
	if (auto component = std::dynamic_pointer_cast<HighScoreComponent>(GetComponent()); component)
	{
		component->FinishName();
	}
}
