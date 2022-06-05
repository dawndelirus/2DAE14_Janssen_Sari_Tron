#pragma once
#include "Command.h"
#include <memory>

namespace dae
{
	enum class ControllerButton
	{
		Empty		= -1,
		DPadUp		= 0x0001,
		DPadDown	= 0x0002,
		DPadLeft	= 0x0004,
		DPadRight	= 0x0008,
		Start		= 0x0010,
		Back		= 0x0020,
		LeftThumb	= 0x0040,
		RightThumb	= 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA		= 0x1000,
		ButtonB		= 0x2000,
		ButtonX		= 0x4000,
		ButtonY		= 0x8000
	};

	enum class KeyboardKey
	{
		Empty = -1,
		
		K_Back		= 0x08,
		K_Tab		= 0x09,
		K_Enter		= 0x0D,

		K_Shift		= 0x10,
		K_Control	= 0x11,
		K_Alt		= 0x12,
		
		K_Esc 		= 0x1B,
		K_SPACE		= 0x20,
		K_Left		= 0x25,
		K_UP 		= 0x26,
		K_RIGHT		= 0x27,
		K_DOWN 		= 0x28,

		K_0 = 0x30,	
		K_1 = 0x31,	
		K_2 = 0x32,	
		K_3 = 0x33,	
		K_4 = 0x34,	
		K_5 = 0x35,	
		K_6 = 0x36,	
		K_7 = 0x37,	
		K_8 = 0x38,	
		K_9 = 0x39,	
		K_A = 0x41,	
		K_B = 0x42,	
		K_C = 0x43,	
		K_D = 0x44,	
		K_E = 0x45,	
		K_F = 0x46,	
		K_G = 0x47,	
		K_H = 0x48,	
		K_I = 0x49,	
		K_J = 0x4A,	
		K_K = 0x4B,	
		K_L = 0x4C,	
		K_M = 0x4D,	
		K_N = 0x4E,	
		K_O = 0x4F,	
		K_P = 0x50,	
		K_Q = 0x51,	
		K_R = 0x52,	
		K_S = 0x53,	
		K_T = 0x54,	
		K_U = 0x55,	
		K_V = 0x56,	
		K_W = 0x57,	
		K_X = 0x58,	
		K_Y = 0x59,	
		K_Z = 0x5A,	
	};

	enum class ButtonState
	{
		pressed,
		downThisFrame,
		releasedThisFrame
	};

	struct InputAction
	{
		InputAction(int idx, ButtonState state, std::shared_ptr<Command> command, ControllerButton button)
			: playerIndex{idx}
			, buttonState{state}
			, command{command}
			, controllerButtonCode{button}
		{}

		InputAction(int idx, ButtonState state, std::shared_ptr<Command> command, KeyboardKey key)
			: playerIndex{idx}
			, buttonState{state}
			, command{command}
			, keyboardKeyCode{ key }
		{}

		InputAction(int idx, ButtonState state, std::shared_ptr<Command> command, ControllerButton button, KeyboardKey key)
			: playerIndex{ idx }
			, buttonState{ state }
			, command{ command }
			, controllerButtonCode{ button }
			, keyboardKeyCode{ key }
		{}

		int playerIndex{ 0 };
		ButtonState buttonState{ ButtonState::pressed };
		std::shared_ptr<Command> command;
		ControllerButton controllerButtonCode{ControllerButton::Empty};
		KeyboardKey keyboardKeyCode{ KeyboardKey::Empty };
	};
}