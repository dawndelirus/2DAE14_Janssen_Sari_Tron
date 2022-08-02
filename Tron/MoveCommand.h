#pragma once
#include <memory>
#include <Command.h>
#include "InputHelpers.h"

class MoveComponent;

class MoveCommand final : public dae::Command
{
public:
	MoveCommand(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MoveComponent> moveComponent, dae::Joystick joystick);
	~MoveCommand() = default;
	void Execute() override;

private:
	std::weak_ptr<MoveComponent> m_MoveComponent;
	dae::Joystick m_Joystick{};
};

