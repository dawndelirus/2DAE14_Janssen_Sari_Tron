#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class SprayComponent final : public dae::BaseComponent, public dae::Subject
{
public:
	SprayComponent(std::weak_ptr<dae::GameObject> gameObject);
	~SprayComponent() = default;

	void SprayPepper();
	//glm::vec2 GetSprayOrigin();
	//glm::vec2 GetSprayOrigin();
private:
	glm::vec2 m_Origin;
	float m_Width;
	float m_Height;
};
