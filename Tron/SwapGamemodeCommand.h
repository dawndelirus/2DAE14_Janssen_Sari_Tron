#pragma once
#include <Command.h>
class SwapGamemodeCommand : public dae::Command
{
public:
	SwapGamemodeCommand(std::shared_ptr<dae::BaseComponent> command);
	~SwapGamemodeCommand() = default;
	void Execute() override;

private:
};

