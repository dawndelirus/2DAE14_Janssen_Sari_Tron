#include "MovementControllerComponent.h"
#include "LevelLayoutComponent.h"

MovementControllerComponent::MovementControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::GameObject> targetObject
	, std::shared_ptr<MoveComponent> moveComponent, std::shared_ptr<LevelPathfindingComponent> pathfinding
	, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_TargetObject(targetObject)
	, m_MoveComponent(moveComponent)
	, m_Pathfinding(pathfinding)
	, m_Layout(levelLayout)
	, m_Path()
{
	auto& position = GetGameObject()->GetWorldPosition();
	m_Path = m_Pathfinding.lock()->FindPath(m_Layout.lock()->GetGridIndex(position), m_Layout.lock()->GetGridIndex(m_TargetObject.lock()->GetWorldPosition()));

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
	auto positionOrigin = m_Layout.lock()->GetGridCenter(m_Path[0]);

	if (abs(m_Target.x - positionOrigin.x) > abs(m_Target.y - positionOrigin.y))
	{
		m_MoveComponent.lock()->Move(m_Target.x - positionOrigin.x, 0.f, 1.f);
	}
	else
	{
		m_MoveComponent.lock()->Move(0.f, -(m_Target.y - positionOrigin.y), 1.f);
	}

	auto positionAfter = GetGameObject()->GetWorldPosition();

	if (abs(positionAfter.x - positionOrigin.x) > abs(positionAfter.x - m_Target.x) 
		|| abs(positionAfter.y - positionOrigin.y) > abs(positionAfter.y - m_Target.y))
	{
		m_Path = m_Pathfinding.lock()->FindPath(m_Layout.lock()->GetGridIndex(positionAfter), m_Layout.lock()->GetGridIndex(m_TargetObject.lock()->GetWorldPosition()));

		if (m_Path.size() <= 1)
		{
			return;
		}

		m_Target = m_Layout.lock()->GetGridCenter(m_Path[1]);
	}
}
