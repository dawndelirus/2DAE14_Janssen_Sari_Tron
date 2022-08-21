#pragma once
#include <Command.h>

class GunComponent;

class FireLeftCommand final : public dae::Command
{
public:
	FireLeftCommand(std::shared_ptr<GunComponent> gunComponent);
	~FireLeftCommand() = default;
	void Execute(float amount) override;
};

