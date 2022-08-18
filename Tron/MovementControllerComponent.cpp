#include "MovementControllerComponent.h"
#include "LevelLayoutComponent.h"

MovementControllerComponent::MovementControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::vector<std::shared_ptr<dae::GameObject>> targetObjects
	, std::shared_ptr<MoveComponent> moveComponent, std::shared_ptr<LevelPathfindingComponent> pathfinding
	, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_TargetObjects()
	, m_MoveComponent(moveComponent)
	, m_Pathfinding(pathfinding)
	, m_Layout(levelLayout)
	, m_Path()
{
	for (size_t i = 0; i < targetObjects.size(); ++i)
	{
		m_TargetObjects.emplace_back(targetObjects[i]);
	}

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
	if (m_Layout.lock()->GetGridIndex(position) == m_Layout.lock()->GetGridIndex(m_Target))
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
	for (size_t i = 0; i < m_TargetObjects.size(); ++i)
	{
		auto& targetPos = m_TargetObjects[i].lock()->GetWorldPosition();
		float cost = m_Pathfinding.lock()->GetHeuristicCost(index, m_Layout.lock()->GetGridIndex(targetPos));
		if (cost < lowestCost)
		{
			lowestCost = cost;
			m_TargetObject = m_TargetObjects[i];
		}
	}

	// get path to closest target
	auto& targetPos = m_TargetObject.lock()->GetWorldPosition();
	m_Path = m_Pathfinding.lock()->FindPath(m_Layout.lock()->GetGridIndex(position), m_Layout.lock()->GetGridIndex(targetPos));
}
