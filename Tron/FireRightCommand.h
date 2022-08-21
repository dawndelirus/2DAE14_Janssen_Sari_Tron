#pragma once
#include <Command.h>

class GunComponent;

class FireRightCommand final : public dae::Command
{
public:
	FireRightCommand(std::shared_ptr<GunComponent> gunComponent);
	~FireRightCommand() = default;
	void Execute(float amount) override;
};

