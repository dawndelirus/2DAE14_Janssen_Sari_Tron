#pragma once
#include "BaseInputManager.h"

namespace dae
{
	class NullInputManager : public BaseInputManager
	{
	public:
		bool ProcessInput() override { return false; };
		void AddInput(InputAction) override {};
		void RemoveInput(InputAction) override {};

		bool IsControllerButton(ButtonState, ControllerButton, int) const override { return false; };
		bool IsKeyboardKey(ButtonState, KeyboardKey) const override { return false; };

		void RemoveControllerInput(ControllerButton, ButtonState, int) override {};
		void RemoveKeyboardInput(KeyboardKey, ButtonState) override {};
	};
}
