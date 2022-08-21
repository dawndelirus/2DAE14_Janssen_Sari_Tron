#pragma once
#include <Command.h>

class PauseCommand final : public dae::Command
{
public:
	PauseCommand(std::shared_ptr<dae::BaseComponent> component);
	~PauseCommand() = default;
	void Execute() override;
};

