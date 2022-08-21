#include "FireUpCommand.h"
#include "GunComponent.h"

FireUpCommand::FireUpCommand(std::shared_ptr<GunComponent> gunComponent)
	: Command(gunComponent)
{
}

void FireUpCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<GunComponent>(GetComponent()); component)
	{
		component->ShootBullet(glm::vec2(0.f, abs(amount)));
	}
}
