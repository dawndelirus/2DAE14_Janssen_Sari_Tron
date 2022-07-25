#pragma once
#include "BaseInputManager.h"
#include "XBox360Controller.h"
#include "KeyboardControls.h"

namespace dae
{
	class InputManager final : public BaseInputManager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		bool ProcessInput() override;
		void AddInput(InputAction action) override;
		void RemoveInput(InputAction action) override;

		glm::vec2 GetJoystickPosition(Joystick stick, int playerIndex = 0) const override;
		bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex = 0) const override;
		bool IsKeyboardKey(ButtonState state, KeyboardKey key) const override;

		void RemoveControllerInput(ControllerButton button, ButtonState state, int playerIndex = 0) override;
		void RemoveKeyboardInput(KeyboardKey button, ButtonState state) override;
	private:

		std::unique_ptr<XBox360Controller> m_Controller{std::make_unique<XBox360Controller>()};
		std::unique_ptr<KeyboardControls> m_Keyboard{std::make_unique<KeyboardControls>()};
	};
}
