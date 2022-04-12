#pragma once
#include "InputHelpers.h"

#include <map>
#include <vector>
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
		//void RemoveInput(XBox360ControllerButton button, ButtonState state);

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
	};
}
