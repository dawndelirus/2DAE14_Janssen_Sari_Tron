#include "MoveCommand.h"
#include "MoveComponent.h"
#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MoveComponent> moveComponent, dae::Joystick joystick)
	: Command(gameObject)
	, m_MoveComponent{ moveComponent }
	, m_Joystick{joystick}
{
}

void MoveCommand::Execute()
{
	const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick);
	m_MoveComponent.lock()->Move(move.x, move.y, move.z);
}
