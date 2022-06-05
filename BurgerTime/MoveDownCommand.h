#pragma once
#include "Command.h"

class MoveDownCommand : public dae::Command
{
public:
	MoveDownCommand(std::shared_ptr<dae::GameObject> gameObject);
	~MoveDownCommand() = default;
	void Execute() override;
};

