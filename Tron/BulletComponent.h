#pragma once
#include <BaseComponent.h>
#include <memory>
#include "Observer.h"

class BulletPoolComponent;
class LevelLayoutComponent;

class BulletComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	friend class BulletPoolComponent;

	enum class Type
	{
		Player,
		Enemy
	};

	~BulletComponent() = default;

	void InitializeBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed, Type bulletSource);
	void SetNext(BulletComponent* next);
	BulletComponent* GetNext();
	bool IsInUse();

	void Update() override;
	void Render() const override {};

	bool GetIsInPool() const;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event);

private:
	BulletComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout);

	void Bounce(const glm::vec3& oldPosition, glm::vec3& newPosition);
	void SetIsInPool(bool isInPool);

	union
	{
		glm::vec2 velocity;
		BulletComponent* next;
	} m_BulletState;

	Type m_Source;
	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;

	float m_BulletSpeed;
	int m_MaxBounces;
	int m_CurrentBounces;

	bool m_IsInPool;
};

