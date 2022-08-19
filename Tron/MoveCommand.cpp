#include "MoveCommand.h"
#include "MoveComponent.h"
#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<MoveComponent> moveComponent, dae::Joystick joystick, int playerIdx)
	: Command(moveComponent)
	, m_Joystick{joystick}
	, m_PlayerIdx{playerIdx}
{
}

void MoveCommand::Execute()
{
	const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick, m_PlayerIdx);
	std::dynamic_pointer_cast<MoveComponent>(GetComponent())->Move(move.x, move.y, move.z);
}
