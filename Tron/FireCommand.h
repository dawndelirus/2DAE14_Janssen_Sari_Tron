#pragma once
#include <Command.h>
#include "InputHelpers.h"

class GunComponent;

class FireCommand : public dae::Command
{
public:
	FireCommand(std::shared_ptr<GunComponent> bulletPool, dae::Joystick joystick);
	~FireCommand() = default;
	void Execute() override;

private:
	dae::Joystick m_Joystick{};
};

