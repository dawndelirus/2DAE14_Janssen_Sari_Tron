#pragma once
#include <BaseComponent.h>
#include "BulletPoolComponent.h"

class BulletComponent final : public dae::BaseComponent
{
public:
	friend class BulletPoolComponent;

	enum class Source
	{
		Player,
		Enemy
	};

	~BulletComponent() = default;

	void InitializeBullet(glm::vec2 startPos, glm::vec2 direction, int bounces, Source bulletSource);
	void SetNext(BulletComponent* next);
	BulletComponent* GetNext();
	bool IsInUse();

	void Update() override;
	void Render() const override {};

private:
	BulletComponent(std::shared_ptr<dae::GameObject> gameObject);

	union
	{
		glm::vec2 velocity;
		BulletComponent* next;
	} m_BulletState;

	Source m_Source;

	int m_MaxBounces;
	int m_CurrentBounces;

};

