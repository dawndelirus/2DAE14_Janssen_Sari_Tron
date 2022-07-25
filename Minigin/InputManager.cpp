#include "MiniginPCH.h"
#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"

bool dae::InputManager::ProcessInput()
{
	m_Controller->Update();
	m_Keyboard->Update();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::AddInput(InputAction action)
{
	if (action.keyboardKeyCode != KeyboardKey::Empty)
	{
		m_Keyboard->AddInput(action.keyboardKeyCode, action.command, action.buttonState);
	}
	if (action.controllerButtonCode != ControllerButton::Empty)
	{
		m_Controller->AddInput(action.controllerButtonCode, action.command, action.buttonState, action.playerIndex);
	}
	if (action.joystickCode != Joystick::Empty)
	{
		m_Controller->AddInput(action.joystickCode, action.command, action.playerIndex);
	}
}

void dae::InputManager::RemoveInput(InputAction action)
{
	if (action.keyboardKeyCode != KeyboardKey::Empty)
	{
		m_Controller->RemoveInput(action.controllerButtonCode, action.buttonState, action.playerIndex);
	}
	if (action.controllerButtonCode != ControllerButton::Empty)
	{
		m_Keyboard->RemoveInput(action.keyboardKeyCode, action.buttonState);
	}
	if (action.joystickCode != Joystick::Empty)
	{
		m_Controller->RemoveInput(action.joystickCode, action.playerIndex);
	}
}

glm::vec2 dae::InputManager::GetJoystickPosition(Joystick stick, int playerIndex) const
{
	return m_Controller->GetJoystickPosition(stick, playerIndex);
}

bool dae::InputManager::IsControllerButton(ButtonState state, ControllerButton button, int playerIndex) const
{
	switch (state)
	{
	case dae::ButtonState::pressed:
		return m_Controller->IsPressed(button, playerIndex);
		break;
	case dae::ButtonState::downThisFrame:
		return m_Controller->IsDownThisFrame(button, playerIndex);
		break;
	case dae::ButtonState::releasedThisFrame:
		return m_Controller->IsReleasedThisFrame(button, playerIndex);
		break;
	}

	return false;
}

bool dae::InputManager::IsKeyboardKey(ButtonState state, KeyboardKey key) const
{
	switch (state)
	{
	case dae::ButtonState::pressed:
		return m_Keyboard->IsPressed(key);
		break;
	case dae::ButtonState::downThisFrame:
		return m_Keyboard->IsDownThisFrame(key);
		break;
	case dae::ButtonState::releasedThisFrame:
		return m_Keyboard->IsReleasedThisFrame(key);
		break;
	}

	return false;
}
