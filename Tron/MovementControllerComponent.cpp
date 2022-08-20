#include "MovementControllerComponent.h"
#include "LevelLayoutComponent.h"
#include "EnemyControllerComponent.h"

MovementControllerComponent::MovementControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<EnemyControllerComponent> controller
	, std::shared_ptr<MoveComponent> moveComponent, std::shared_ptr<LevelPathfindingComponent> pathfinding
	, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_TargetObject()
	, m_MoveComponent(moveComponent)
	, m_Pathfinding(pathfinding)
	, m_Layout(levelLayout)
	, m_Controller(controller)
	, m_Path()
	, m_Target()
{
	GetPathToClosestTarget();

	if (m_Path.size() < 1)
	{
		return;
	}

	m_Target = m_Layout.lock()->GetGridCenter(m_Path[1]);
}

void MovementControllerComponent::Update()
{
	MoveToTarget();
}


void MovementControllerComponent::MoveToTarget()
{
	auto originPath = m_Layout.lock()->GetGridCenter(m_Path[0]);

	// Move on x or on y depending on distance
	if (abs(m_Target.x - originPath.x) > abs(m_Target.y - originPath.y))
	{
		m_MoveComponent.lock()->Move(m_Target.x - originPath.x, 0.f, 1.f);
	}
	else
	{
		m_MoveComponent.lock()->Move(0.f, -(m_Target.y - originPath.y), 1.f);
	}

	auto& position = GetGameObject()->GetWorldPosition();

	// Recalculate target
	if (m_Layout.lock()->GetGridIndex(position) == m_Layout.lock()->GetGridIndex(m_Target) || m_Path.size() <= 1 || !IsStillOnPath() || m_TargetObject.lock() == nullptr)
	{
		GetPathToClosestTarget();

		if (m_Path.size() <= 1)
		{
			return;
		}

		m_Target = m_Layout.lock()->GetGridCenter(m_Path[1]);
	}
}

std::shared_ptr<dae::GameObject> MovementControllerComponent::GetTarget() const
{
	return m_TargetObject.lock();
}

void MovementControllerComponent::GetPathToClosestTarget()
{
	auto& position = GetGameObject()->GetWorldPosition();
	int index = m_Layout.lock()->GetGridIndex(position);
	
	// get closest target using manhattan distance from pathfinding
	float lowestCost{FLT_MAX};
	auto& targets = m_Controller.lock()->GetTargets();
	for (size_t i = 0; i < targets.size(); ++i)
	{
		auto& targetPos = targets[i].lock()->GetWorldPosition();
		float cost = m_Pathfinding.lock()->GetHeuristicCost(index, m_Layout.lock()->GetGridIndex(targetPos));
		if (cost < lowestCost)
		{
			lowestCost = cost;
			m_TargetObject = targets[i];
		}
	}

	if (targets.size() == 0)
	{
		m_TargetObject.reset();
		return;
	}

	// get path to closest target
	auto& targetPos = m_TargetObject.lock()->GetWorldPosition();
	m_Path = m_Pathfinding.lock()->FindPath(m_Layout.lock()->GetGridIndex(position), m_Layout.lock()->GetGridIndex(targetPos));
}

bool MovementControllerComponent::IsStillOnPath()
{
	auto& position = GetGameObject()->GetWorldPosition();
	int index = m_Layout.lock()->GetGridIndex(position);

	return m_Path.size() > 0 && (index == m_Path[0] || index == m_Path[1]);
}