#include "EnemyTankComponent.h"
#include "ObserverHelpers.h"
#include "MovementControllerComponent.h"
#include "LevelLayoutComponent.h"
#include "GunComponent.h"

EnemyTankComponent::EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MovementControllerComponent> movementController
	, std::shared_ptr<GunComponent> gunComp, std::shared_ptr<LevelLayoutComponent> levelLayout, int points)
	: BaseComponent(gameObject)
	, m_MovementController{movementController}
	, m_LevelLayout{levelLayout}
	, m_Gun{gunComp}
	, m_Points{points}
	, m_IsDead{false}
{
}

void EnemyTankComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
	}

	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		m_IsDead = true;
		Subject::Notify(gameObject, std::make_shared<EnemyKilledObserverEvent>(m_Points));
	}
}

void EnemyTankComponent::Update()
{
	if (m_IsDead)
	{
		return;
	}

	auto target = m_MovementController.lock()->GetTarget();

	int idxCurrent = m_LevelLayout.lock()->GetGridIndex(GetGameObject()->GetWorldPosition());
	int idxTarget = m_LevelLayout.lock()->GetGridIndex(target->GetWorldPosition());
	int gridWidth = m_LevelLayout.lock()->GetGridWidth();

	bool isHorizontal{};
	if (static_cast<int>(static_cast<float>(idxCurrent) / static_cast<float>(gridWidth))
		== static_cast<int>(static_cast<float>(idxTarget) / static_cast<float>(gridWidth)))
	{
		// target is on same horizontal line
		isHorizontal = true;
	}
	else if (idxCurrent % gridWidth	== idxTarget % gridWidth)
	{
		// target is on same vertical line
		isHorizontal = false;
	}
	else
	{
		return;
	}

	if (IsWholePathWalkable(idxCurrent, idxTarget, isHorizontal))
	{
		auto direction = m_LevelLayout.lock()->GetGridCenter(idxTarget) - m_LevelLayout.lock()->GetGridCenter(idxCurrent);
		
		m_Gun.lock()->ShootBullet(direction);
	}
}

bool EnemyTankComponent::IsWholePathWalkable(int idxCurrent, int idxTarget, bool isHorizontal)
{
	int increment{ 1 };
	int length{};
	int start{};

	if (!isHorizontal)
	{
		increment = m_LevelLayout.lock()->GetGridWidth();
	}

	if (idxCurrent > idxTarget)
	{
		start = idxTarget;
		length = idxCurrent;
	}
	else
	{
		start = idxCurrent;
		length = idxTarget;
	}

	bool isNotWalkable{false};
	for (int i = start; i < length; i += increment)
	{
		isNotWalkable = isNotWalkable || !m_LevelLayout.lock()->IsWalkable(i);
	}

	return !isNotWalkable;
}
