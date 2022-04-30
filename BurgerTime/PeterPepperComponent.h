#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class PeterPepperComponent final : public dae::BaseComponent
{
public:
	PeterPepperComponent(std::shared_ptr<dae::GameObject> gameObject);
	~PeterPepperComponent() = default;

	virtual void Update();
	virtual void Render() const;



private:
};

