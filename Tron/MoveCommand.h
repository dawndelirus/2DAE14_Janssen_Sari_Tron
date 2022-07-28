#pragma once
#include <Command.h>
#include "InputHelpers.h"

class MoveComponent;

class MoveCommand final : public dae::Command
{
public:
	MoveCommand(std::shared_ptr<dae::GameObject> pGameObject, MoveComponent* pMoveComponent, dae::Joystick joystick);
	~MoveCommand() = default;
	void Execute() override;

private:
	MoveComponent* m_pMoveComponent;
	dae::Joystick m_Joystick{};
};

