#include "MiniginPCH.h"
#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"

bool dae::InputManager::ProcessInput()
{
	//for (size_t i{}; i < m_Controllers.size(); ++i)
	//{
	//	m_Controllers[i]->Update();
	//}

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
	//if (action.playerIndex >= m_Controllers.size())
	//{
	//	m_Controllers.emplace(std::make_pair(action.playerIndex, std::make_unique<XBox360Controller>(action.playerIndex)));
	//}
	//m_Controllers[action.playerIndex]->AddInput(action.controllerButtonCode, action.command, action.buttonState);

	m_Controller->AddInput(action.controllerButtonCode, action.command, action.buttonState, action.playerIndex);
}

//void dae::InputManager::RemoveInput(int actionId)
//{
//	for (auto& actions : m_InputActions)
//	{
//		if (actions.first != actionId)
//		{
//			continue;
//		}
//		m_Controller->RemoveInput(actions.second.actionID);
//		m_InputActions.erase(actionId);
//	}
//}

bool dae::InputManager::IsControllerButton(ButtonState state, ControllerButton button, int playerIndex)
{
	switch (state)
	{
	case dae::ButtonState::pressed:
		return m_Controller->IsPressed(button, playerIndex);
		//m_Controllers.at(playerIndex)->IsPressed(button);
		break;
	case dae::ButtonState::downThisFrame:
		return m_Controller->IsDownThisFrame(button, playerIndex);
		//m_Controllers.at(playerIndex)->IsDownThisFrame(button);
		break;
	case dae::ButtonState::releasedThisFrame:
		return m_Controller->IsReleasedThisFrame(button, playerIndex);
		//m_Controllers.at(playerIndex)->IsReleasedThisFrame(button);
		break;
	}

	return false;
}