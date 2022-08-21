#include "SwapGamemodeCommand.h"
#include "TextComponent.h"
#include "LevelManager.h"

SwapGamemodeCommand::SwapGamemodeCommand(std::shared_ptr<dae::BaseComponent> command)
	: Command(command)
{
}

void SwapGamemodeCommand::Execute(float )
{
	if (auto component = std::dynamic_pointer_cast<dae::TextComponent>(GetComponent()); component != nullptr)
	{
		auto gameMode = LevelManager::GetInstance().CycleGameMode();
		switch (gameMode)
		{
		case LevelManager::GameMode::Single:
			component->SetText("Singleplayer");
			break;
		case LevelManager::GameMode::Coop:
			component->SetText("Co-op");
			break;
		case LevelManager::GameMode::Versus:
			component->SetText("Versus");
			break;
		default:
			break;
		}
	}
}
