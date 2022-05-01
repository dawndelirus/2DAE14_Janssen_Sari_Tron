#include "MiniginPCH.h"
#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"

bool dae::InputManager::ProcessInput()
{
	m_Controller->Update();

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
	m_Controller->AddInput(action.controllerButtonCode, action.command, action.buttonState, action.playerIndex);
}

void dae::InputManager::RemoveInput(InputAction action)
{
	RemoveInput(action.controllerButtonCode, action.buttonState, action.playerIndex);
}

void dae::InputManager::RemoveInput(ControllerButton button, ButtonState state, int playerIndex)
{
	m_Controller->RemoveInput(button, state, playerIndex);
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