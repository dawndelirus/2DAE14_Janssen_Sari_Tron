#include "FireDownCommand.h"
#include "GunComponent.h"

FireDownCommand::FireDownCommand(std::shared_ptr<GunComponent> gunComponent)
	: Command(gunComponent)
{
}

void FireDownCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<GunComponent>(GetComponent()); component)
	{
		component->ShootBullet(glm::vec2(0.f, abs(amount)));
	}
}
