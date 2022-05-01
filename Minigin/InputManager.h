#pragma once
#include "InputManagerBase.h"
#include "XBox360Controller.h"

namespace dae
{
	class InputManager final : public InputManagerBase
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		bool ProcessInput() override;
		void AddInput(InputAction action) override;
		void RemoveInput(InputAction action) override;
		void RemoveInput(ControllerButton button, ButtonState state, int playerIndex) override;

		bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex) const override;

	private:
		std::unique_ptr<XBox360Controller> m_Controller{std::make_unique<XBox360Controller>()};
	};
}
