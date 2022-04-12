#pragma once
#include "InputHelpers.h"

#include <unordered_map>
#include <vector>
#include <memory>

namespace dae
{
	class Controller final
	{
	public:
		Controller();
		~Controller();

		void Update();
		void AddInput(ControllerButton button, std::shared_ptr<Command> command, ButtonState state, int playerIndex);
		//void RemoveInput(ControllerButton button, ButtonState state);

		bool IsPressed(ControllerButton button, int playerIndex) const;
		bool IsDownThisFrame(ControllerButton button, int playerIndex) const;
		bool IsReleasedThisFrame(ControllerButton button, int playerIndex) const;
	private:
		class ControllerImpl;
		ControllerImpl* m_pImpl;

		using PlayerButton = std::pair<unsigned, ControllerButton>;
		using ControllerCommandsMap = std::unordered_map<PlayerButton, std::shared_ptr<Command>>;
		ControllerCommandsMap m_CommandsPressed{};
		ControllerCommandsMap m_CommandsDown{};
		ControllerCommandsMap m_CommandsReleased{};
	};
}
