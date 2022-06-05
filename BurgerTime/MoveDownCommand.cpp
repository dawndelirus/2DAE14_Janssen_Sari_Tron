#include "BurgerTimePCH.h"
#include "MoveDownCommand.h"
#include "MoveComponent.h"

MoveDownCommand::MoveDownCommand(std::shared_ptr<dae::GameObject> gameObject)
	: Command(gameObject)
{
}

void MoveDownCommand::Execute()
{
	GetGameObject()->GetComponent<MoveComponent>()->Move(glm::vec2{ 0.f, 1.f });
}
