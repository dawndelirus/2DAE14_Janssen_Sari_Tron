#pragma once
#include "InputHelpers.h"
#include <map>
#include <memory>

namespace dae
{
	class KeyboardControls
	{
	public:
		KeyboardControls();
		~KeyboardControls();

		void Update();
		void AddInput(KeyboardKey key, std::shared_ptr<Command> command, ButtonState state);
		void RemoveInput(KeyboardKey key, ButtonState state);

		bool IsPressed(KeyboardKey key) const;
		bool IsDownThisFrame(KeyboardKey key) const;
		bool IsReleasedThisFrame(KeyboardKey key) const;
	private:
		class KeyboardControlsImpl;
		KeyboardControlsImpl* m_pImpl;

		using KeyboardControlsCommandsMap = std::map<KeyboardKey, std::shared_ptr<Command>>;
		KeyboardControlsCommandsMap m_CommandsPressed{};
		KeyboardControlsCommandsMap m_CommandsDown{};
		KeyboardControlsCommandsMap m_CommandsReleased{};
	};
}
