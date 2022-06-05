#pragma once
#include "Command.h"

class MoveLeftCommand : public dae::Command
{
public:
	MoveLeftCommand(std::shared_ptr<dae::GameObject> gameObject);
	~MoveLeftCommand() = default;
	void Execute() override;
};

