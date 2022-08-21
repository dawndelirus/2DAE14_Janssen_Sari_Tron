#pragma once
#include <Command.h>

class MoveComponent;

class MoveLeftCommand final : public dae::Command
{
public:
	MoveLeftCommand(std::shared_ptr<MoveComponent> moveComponent);
	~MoveLeftCommand() = default;
	void Execute(float amount) override;
};

