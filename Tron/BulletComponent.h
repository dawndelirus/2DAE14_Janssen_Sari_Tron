#pragma once
#include <BaseComponent.h>

class BulletPoolComponent;

class BulletComponent final : public dae::BaseComponent
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

private:
	BulletComponent(std::shared_ptr<dae::GameObject> gameObject);

	union
	{
		glm::vec2 velocity;
		BulletComponent* next;
	} m_BulletState;

	Type m_Source;

	float m_BulletSpeed;
	int m_MaxBounces;
	int m_CurrentBounces;
};

