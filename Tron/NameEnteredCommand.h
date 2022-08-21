#pragma once
#include <Command.h>

class HighScoreComponent;

class NameEnteredCommand final : public dae::Command
{
public:
	NameEnteredCommand(std::shared_ptr<HighScoreComponent> component);
	~NameEnteredCommand() = default;

	void Execute(float amount) override;
};

