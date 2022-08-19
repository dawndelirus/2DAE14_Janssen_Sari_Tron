#pragma once
#include <memory>
#include <Command.h>
#include "InputHelpers.h"

class MoveComponent;

class MoveCommand final : public dae::Command
{
public:
	MoveCommand(std::shared_ptr<MoveComponent> moveComponent, dae::Joystick joystick, int playerIdx);
	~MoveCommand() = default;
	void Execute() override;

private:
	dae::Joystick m_Joystick{};
	int m_PlayerIdx;
};

