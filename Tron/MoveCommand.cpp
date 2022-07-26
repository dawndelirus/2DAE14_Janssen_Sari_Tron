#include "MoveCommand.h"
#include "MoveComponent.h"
#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<dae::GameObject> pGameObject, dae::Joystick joystick)
	: Command(pGameObject)
	, m_Joystick{joystick}
{
}

void MoveCommand::Execute()
{
	const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick);
	GetGameObject()->GetComponent<MoveComponent>()->Move(move.x, move.y, move.z);
}
