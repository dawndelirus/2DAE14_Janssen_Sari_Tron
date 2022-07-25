#include "MoveCommand.h"
#include "MoveComponent.h"
//#include "ServiceLocator.h" 

MoveCommand::MoveCommand(std::shared_ptr<dae::GameObject> pGameObject)
	: Command(pGameObject)
{
}

void MoveCommand::Execute()
{
	//const auto& move = dae::ServiceLocator::GetInputManager().GetJoystickPosition(dae::Joystick::LeftStick);
	//GetGameObject()->GetComponent<MoveComponent>()->Move(move.x, move.y);
	GetGameObject()->GetComponent<MoveComponent>()->Move(0.f, 0.f);
}
