#pragma once
#include <Command.h>

class MoveComponent;

class MoveRightCommand final : public dae::Command
{
public:
	MoveRightCommand(std::shared_ptr<MoveComponent> moveComponent);
	~MoveRightCommand() = default;
	void Execute(float amount) override;
};

