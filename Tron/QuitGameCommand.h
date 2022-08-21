#pragma once
#include <Command.h>
class QuitGameCommand final: public dae::Command
{
public:
	QuitGameCommand(std::shared_ptr<dae::BaseComponent> component);
	~QuitGameCommand() = default;

	void Execute(float amount) override;
};

