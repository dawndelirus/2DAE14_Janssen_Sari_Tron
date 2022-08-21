#pragma once
#include <Command.h>

class GunComponent;

class FireDownCommand final : public dae::Command
{
public:
	FireDownCommand(std::shared_ptr<GunComponent> gunComponent);
	~FireDownCommand() = default;
	void Execute(float amount) override;
};
