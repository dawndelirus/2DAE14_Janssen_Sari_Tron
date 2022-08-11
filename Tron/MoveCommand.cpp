#include "MoveCommand.h"
#include "MoveComponent.h"
#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<MoveComponent> moveComponent, dae::Joystick joystick)
	: Command(moveComponent)
	, m_Joystick{joystick}
{
}

void MoveCommand::Execute()
{
	const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick);
	std::dynamic_pointer_cast<MoveComponent>(GetComponent())->Move(move.x, move.y, move.z);
}
