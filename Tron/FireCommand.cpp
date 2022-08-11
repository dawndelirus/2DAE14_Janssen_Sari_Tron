#include "FireCommand.h"
#include "GunComponent.h"
#include "ServiceLocator.h"

FireCommand::FireCommand(std::shared_ptr<GunComponent> bulletPool, dae::Joystick joystick)
	: Command(bulletPool)
	, m_Joystick(joystick)
{
}

void FireCommand::Execute()
{
	const auto& joystickPos = dae::ServiceLocator::GetInputManager().GetJoystickPosition(m_Joystick);
	std::dynamic_pointer_cast<GunComponent>(GetComponent())->ShootBullet(glm::vec2(joystickPos.x, -joystickPos.y));
}
