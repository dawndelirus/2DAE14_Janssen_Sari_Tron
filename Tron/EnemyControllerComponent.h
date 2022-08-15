#pragma once
#include <BaseComponent.h>

class EnemyControllerComponent : public dae::BaseComponent
{
public:
	EnemyControllerComponent(std::shared_ptr<dae::GameObject> gameObject);
	~EnemyControllerComponent() = default;

	void AddEnemies(std::shared_ptr<dae::GameObject> enemy);

	void Update() override;
	void Render() const override {};

private:
	std::vector<std::weak_ptr<dae::GameObject>> m_Enemies;
};

