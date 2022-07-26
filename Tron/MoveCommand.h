#pragma once
#include <Command.h>
#include "InputHelpers.h"

class MoveCommand final : public dae::Command
{
public:
	MoveCommand(std::shared_ptr<dae::GameObject> pGameObject, dae::Joystick joystick);
	~MoveCommand() = default;
	void Execute() override;

private:
	dae::Joystick m_Joystick{};
};

