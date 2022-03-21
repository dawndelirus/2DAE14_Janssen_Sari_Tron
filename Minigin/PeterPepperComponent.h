#pragma once
#include "BaseComponent.h"
#include "GameObject.h"

class PeterPepperComponent : public dae::BaseComponent
{
public:
	PeterPepperComponent(std::shared_ptr<dae::GameObject> gameObject);
	~PeterPepperComponent() = default;

private:

};

