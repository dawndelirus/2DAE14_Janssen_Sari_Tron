#include "BurgerTimePCH.h"
#include "MoveUpCommand.h"
#include "MoveComponent.h"

MoveUpCommand::MoveUpCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void MoveUpCommand::Execute()
{
	GetGameObject()->GetComponent<MoveComponent>()->Move(glm::vec2{ 0.f, -1.f });
}
