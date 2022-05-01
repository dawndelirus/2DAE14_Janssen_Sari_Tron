#pragma once
#include "InputManagerBase.h"

namespace dae
{
	class NullInputManager : public InputManagerBase
	{
	public:
		bool ProcessInput() override { return false; };
		void AddInput(InputAction) override {};
		void RemoveInput(InputAction) override {};
		void RemoveInput(ControllerButton, ButtonState, int) override {};

		bool IsControllerButton(ButtonState, ControllerButton, int) const override { return false; };
	};
}
