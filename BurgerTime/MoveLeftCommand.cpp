#include "BurgerTimePCH.h"
#include "MoveLeftCommand.h"
#include "MoveComponent.h"

MoveLeftCommand::MoveLeftCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void MoveLeftCommand::Execute()
{
	GetGameObject()->GetComponent<MoveComponent>()->Move(glm::vec2{-1.f, 0.f});
}
