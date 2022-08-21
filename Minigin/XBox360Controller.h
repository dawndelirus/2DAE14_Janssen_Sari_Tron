#pragma once
#include "InputHelpers.h"
#include <map>
#include <memory>

namespace dae
{
	class XBox360Controller final
	{
	public:
		XBox360Controller();
		~XBox360Controller();

		void Update();
		void AddInput(ControllerButton button, std::shared_ptr<Command> command, ButtonState state, int playerIndex);
		void AddInput(Joystick stick, JoystickState state, std::shared_ptr<Command> command, int playerIndex);
		
		void RemoveInput(ControllerButton button, ButtonState state, int playerIndex);
		void RemoveInput(Joystick stick, JoystickState state, int playerIndex);

		glm::vec3 GetJoystickPosition(Joystick stick, int playerIndex) const;
		bool IsPressed(ControllerButton button, int playerIndex) const;
		bool IsDownThisFrame(ControllerButton button, int playerIndex) const;
		bool IsReleasedThisFrame(ControllerButton button, int playerIndex) const;

	private:
		class XBox360ControllerImpl;
		XBox360ControllerImpl* m_pImpl;

		using PlayerButton = std::pair<unsigned, ControllerButton>;
		using XBox360ControllerCommandsMap = std::map<PlayerButton, std::shared_ptr<Command>>;
		XBox360ControllerCommandsMap m_CommandsPressed{};
		XBox360ControllerCommandsMap m_CommandsDown{};
		XBox360ControllerCommandsMap m_CommandsReleased{};

		using PlayerJoystick = std::pair<int, Joystick>;
		std::map<PlayerJoystick, std::shared_ptr<Command>> m_CommandsJoystickPositive{};
		std::map<PlayerJoystick, std::shared_ptr<Command>> m_CommandsJoystickNegative{};
	};
}
