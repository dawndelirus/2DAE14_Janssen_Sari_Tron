#pragma once
#include "Command.h"

class MoveRightCommand : public dae::Command
{
public:
	MoveRightCommand(std::shared_ptr<dae::GameObject> gameObject);
	~MoveRightCommand() = default;
	void Execute() override;
};

