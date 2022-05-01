#pragma once
#include "InputHelpers.h"

namespace dae
{
	class InputManagerBase
	{
	public:
		InputManagerBase() = default;
		virtual ~InputManagerBase() = default;

		InputManagerBase(const InputManagerBase& other) = delete;
		InputManagerBase(InputManagerBase&& other) = delete;
		InputManagerBase& operator=(const InputManagerBase& other) = delete;
		InputManagerBase& operator=(InputManagerBase&& other) = delete;

		virtual bool ProcessInput() = 0;
		virtual void AddInput(InputAction action) = 0;
		virtual void RemoveInput(InputAction action) = 0;
		virtual void RemoveInput(ControllerButton button, ButtonState state, int playerIndex) = 0;

		virtual bool IsControllerButton(ButtonState state, ControllerButton button, int playerIndex) const = 0;
	};
}
