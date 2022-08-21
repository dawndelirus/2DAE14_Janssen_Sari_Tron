#include "FireLeftCommand.h"
#include "GunComponent.h"

FireLeftCommand::FireLeftCommand(std::shared_ptr<GunComponent> gunComponent)
	: Command(gunComponent)
{
}

void FireLeftCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<GunComponent>(GetComponent()); component)
	{
		component->ShootBullet(glm::vec2(-abs(amount), 0.f));
	}
}
