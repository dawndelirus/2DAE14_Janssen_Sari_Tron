#pragma once
#include "Command.h"
#include "GameObject.h"
#include <memory>

class TakeDamageCommand : public dae::Command
{
public:
	TakeDamageCommand(std::shared_ptr<dae::GameObject> gameObject);
	~TakeDamageCommand() = default;
	void Execute() override;
};

