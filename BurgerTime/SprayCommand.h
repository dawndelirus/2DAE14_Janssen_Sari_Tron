#pragma once
#include "Command.h"

class SprayCommand final : public dae::Command
{
public:
	SprayCommand(std::shared_ptr<dae::GameObject> pGameObject);
	~SprayCommand() = default;
	void Execute();
};
