#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

class MovementControllerComponent;
class LevelLayoutComponent;
class GunComponent;

class EnemyTankComponent : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MovementControllerComponent> movementController
		, std::shared_ptr<GunComponent> gunComp, std::shared_ptr<LevelLayoutComponent> levelLayout, int points);
	~EnemyTankComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override;
	void Render() const override {}

private:
	bool IsWholePathWalkable(int idxCurrent, int idxTarget, bool isHorizontal);

	std::weak_ptr<MovementControllerComponent> m_MovementController;
	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;
	std::weak_ptr<GunComponent> m_Gun;

	int m_Points;
	bool m_IsDead;
};

