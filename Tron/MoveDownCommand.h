#pragma once
#include <Command.h>

class MoveComponent;

class MoveDownCommand final : public dae::Command
{
public:
	MoveDownCommand(std::shared_ptr<MoveComponent> moveComponent);
	~MoveDownCommand() = default;
	void Execute(float amount) override;
};
