#pragma once
#include <Command.h>


class MoveCommand : public dae::Command
{
public:
	MoveCommand(std::shared_ptr<dae::GameObject> pGameObject);
	~MoveCommand() = default;
	void Execute() override;
};

