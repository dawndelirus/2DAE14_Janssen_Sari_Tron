#include "BurgerTimePCH.h"
#include "MoveRightCommand.h"
#include "MoveComponent.h"

MoveRightCommand::MoveRightCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void MoveRightCommand::Execute()
{
	GetGameObject()->GetComponent<MoveComponent>()->Move(glm::vec2{ 1.f, 0.f });
}
