#pragma once
#include <Command.h>

class StartGameCommand final : public dae::Command
{
public: 
	StartGameCommand(std::shared_ptr<dae::BaseComponent> component);
	~StartGameCommand() = default;
	void Execute() override;
};

