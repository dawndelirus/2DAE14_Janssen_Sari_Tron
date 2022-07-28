#include "MoveCommand.h"
#include "MoveComponent.h"
#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<dae::GameObject> pGameObject, MoveComponent* pMoveComponent, dae::Joystick joystick)
	: Command(pGameObject)
	, m_pMoveComponent{pMoveComponent}
	, m_Joystick{joystick}
{
}

void MoveCommand::Execute()
{
	const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick);
	m_pMoveComponent->Move(move.x, move.y, move.z);
}
