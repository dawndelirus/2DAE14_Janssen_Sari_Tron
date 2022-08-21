#pragma once
#include <Command.h>

class MoveComponent;

class MoveUpCommand final : public dae::Command
{
public:
	MoveUpCommand(std::shared_ptr<MoveComponent> moveComponent);
	~MoveUpCommand() = default;
	void Execute(float amount) override;
};

