#pragma once
#include "InputHelpers.h"

namespace dae
{
	class BaseInputManager
	{
	public:
		BaseInputManager() = default;
		virtual ~BaseInputManager() = default;

		BaseInputManager(const BaseInputManager& other) = delete;
		BaseInputManager(BaseInputManager&& other) = delete;
		BaseInputManager& operator=(const BaseInputManager& other) = delete;
		BaseInputManager& operator=(BaseInputManager&& other) = delete;

		virtual bool ProcessInput() = 0;
		virtual void AddInput(InputAction action) = 0;
		virtual void RemoveInput(InputAction action) = 0;
		virtual void Quit() = 0;

		virtual glm::vec3 GetJoystickPosition(Joystick stick, int playerIndex = 0) const = 0;
		virtual bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex) const = 0;
		virtual bool IsKeyboardKey(ButtonState state, KeyboardKey key) const = 0;
	};
}
