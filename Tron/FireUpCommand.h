#pragma once
#include <Command.h>

class GunComponent;

class FireUpCommand final : public dae::Command
{
public:
	FireUpCommand(std::shared_ptr<GunComponent> gunComponent);
	~FireUpCommand() = default;
	void Execute(float amount) override;
};

