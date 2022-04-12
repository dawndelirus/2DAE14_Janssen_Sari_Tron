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
		void RemoveInput(InputAction action);
		void RemoveInput(ControllerButton button, ButtonState state, int playerIndex);

		bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex);

	private:
		std::unique_ptr<XBox360Controller> m_Controller{std::make_unique<XBox360Controller>()};
	};

}
