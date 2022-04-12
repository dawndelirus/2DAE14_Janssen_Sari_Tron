#pragma once
#include "Singleton.h"
#include "InputHelpers.h"
#include "XBox360Controller.h"
#include <map>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddInput(InputAction action);
		//void RemoveInput(InputAction action);

		bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex);

	private:
		//using ControllerKey = std::pair<int, ControllerButton>; // actionId, ControllerButtons
		//using ControllerCommandsMap = std::map<ControllerKey, std::shared_ptr<Command>>;
		//ControllerCommandsMap m_ConsoleCommands{};
		//std::map<int,std::unique_ptr<XBox360Controller>> m_Controllers{};

		std::unique_ptr<XBox360Controller> m_Controller{std::make_unique<XBox360Controller>()};
	};

}
