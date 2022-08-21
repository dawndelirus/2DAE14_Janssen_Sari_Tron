#pragma once
#include <Command.h>

class ResumeCommand final : public dae::Command
{
public:
	ResumeCommand(std::shared_ptr<dae::BaseComponent> component);
	~ResumeCommand() = default;
	void Execute(float amount) override;

};

