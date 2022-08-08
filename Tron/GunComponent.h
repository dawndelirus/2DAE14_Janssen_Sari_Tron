#pragma once
#include <BaseComponent.h>

class GunComponent final : public dae::BaseComponent
{
public:
	GunComponent(std::shared_ptr<dae::GameObject> gameObject);
	~GunComponent() = default;

	void Update() override {};
	void Render() const override {};

private:
};

