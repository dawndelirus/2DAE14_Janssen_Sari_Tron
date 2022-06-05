#pragma once
#include "Command.h"

class MoveUpCommand : public dae::Command
{
public:
	MoveUpCommand(std::shared_ptr<dae::GameObject> gameObject);
	~MoveUpCommand() = default;
	void Execute() override;
};

