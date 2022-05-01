#include "MiniginPCH.h"
#include "XBox360Controller.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <array>
#include <vector>
#include "Clock.h"

#pragma comment(lib, "xinput.lib")

using namespace dae;

class XBox360Controller::XBox360ControllerImpl
{
public:
	XBox360ControllerImpl()
	{
		for (int i{}; i < XUSER_MAX_COUNT; ++i)
		{
			// reset input of all controllers
			ZeroMemory(&m_PreviousState[i], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[i], sizeof(XINPUT_STATE));

			// set all controllers to not connected
			m_IsControllerConnected.resize(XUSER_MAX_COUNT, false);
		}
	}

	void Update()
	{
		bool hasTimePassed = HasTimePassed();

		for (int cIdx{}; cIdx < XUSER_MAX_COUNT; ++cIdx)
		{
			// check if controller is connected
			bool isConnected = m_IsControllerConnected[cIdx];
			if (!hasTimePassed && !isConnected)
			{
				// every second, check all controller connections for added or removed controllers
				continue;
			}

			// reset states
			CopyMemory(&m_PreviousState[cIdx], &m_CurrentState[cIdx], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[cIdx], sizeof(XINPUT_STATE));

			// get current state of controller
			XINPUT_STATE state;
			DWORD result = XInputGetState(cIdx, &state);

			if (result == ERROR_SUCCESS)
			{
				if (!isConnected)
				{
					// is connected but wasn't connected -> add
					ConnectController(cIdx);
				}

				// set buttons released and pressed
				CopyMemory(&m_CurrentState[cIdx], &state, sizeof(XINPUT_STATE));

				auto buttonChanges = m_CurrentState[cIdx].Gamepad.wButtons ^ m_PreviousState[cIdx].Gamepad.wButtons;
				m_ButtonsPressedThisFrame[cIdx] = buttonChanges & m_CurrentState[cIdx].Gamepad.wButtons;
				m_ButtonsReleasedThisFrame[cIdx] = buttonChanges & (~m_CurrentState[cIdx].Gamepad.wButtons);

			}
			else
			{
				if (isConnected)
				{
					// was connected but not anymore -> remove
					DisConnectController(cIdx);
				}

				// reset state of controller buttons
				ZeroMemory(&m_CurrentState[cIdx], sizeof(XINPUT_STATE));
				ZeroMemory(&m_PreviousState[cIdx], sizeof(XINPUT_STATE));
				continue;
			}
		}
	}

	bool IsPressed(ControllerButton button, int playerIdx) const
	{
		if (playerIdx == -1)
		{
			return false;
		}
		return m_CurrentState[playerIdx].Gamepad.wButtons & static_cast<unsigned int>(button);
	}

	bool IsDownThisFrame(ControllerButton button, int playerIdx) const
	{
		if (playerIdx == -1)
		{
			return false;
		}
		return m_ButtonsPressedThisFrame[playerIdx] & static_cast<unsigned int>(button);
	}

	bool IsReleasedThisFrame(ControllerButton button, int playerIdx) const
	{
		if (playerIdx == -1)
		{
			return false;
		}
		return m_ButtonsReleasedThisFrame[playerIdx] & static_cast<unsigned int>(button);
	}

private:
	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_CurrentState{};
	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_PreviousState{};

	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsPressedThisFrame{};
	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsReleasedThisFrame{};

	std::vector<bool> m_IsControllerConnected{};
	float m_TotalTime{ 1.f };
	float m_CurrentTime{};

	bool HasTimePassed()
	{
		if (m_CurrentTime >= m_TotalTime)
		{
			m_CurrentTime = 0.f;
			return true;
		}

		m_CurrentTime += dae::Clock::GetDeltaTime();
		return false;
	}

	void ConnectController(int cIdx)
	{
		m_IsControllerConnected[cIdx] = true;
	}

	void DisConnectController(int cIdx)
	{
		m_IsControllerConnected[cIdx] = false;
	}
};

XBox360Controller::XBox360Controller()
{
	m_pImpl = new XBox360ControllerImpl();
}

XBox360Controller::~XBox360Controller()
{
	delete m_pImpl;
}

bool XBox360Controller::IsPressed(ControllerButton button, int playerIndex) const
{
	return m_pImpl->IsPressed(button, playerIndex);
}

bool XBox360Controller::IsDownThisFrame(ControllerButton button, int playerIndex) const
{
	return m_pImpl->IsDownThisFrame(button, playerIndex);
}

bool XBox360Controller::IsReleasedThisFrame(ControllerButton button, int playerIndex) const
{
	return m_pImpl->IsReleasedThisFrame(button, playerIndex);
}

void XBox360Controller::Update()
{
	m_pImpl->Update();

	for (auto& command : m_CommandsPressed)
	{
		const PlayerButton& playerButton = command.first;
		if (IsPressed(playerButton.second, playerButton.first))
		{
			command.second->Execute();
		}
	}

	for (auto& command : m_CommandsDown)
	{
		const PlayerButton& playerButton = command.first;
		if (IsDownThisFrame(playerButton.second, playerButton.first))
		{
			command.second->Execute();
		}
	}

	for (auto& command : m_CommandsReleased)
	{
		const PlayerButton& playerButton = command.first;
		if (IsReleasedThisFrame(playerButton.second, playerButton.first))
		{
			command.second->Execute();
		}
	}
}

void XBox360Controller::AddInput(ControllerButton button, std::shared_ptr<Command> command, ButtonState state, int playerIndex)
{
	switch (state)
	{
	case ButtonState::pressed:
		m_CommandsPressed.emplace(std::make_pair(playerIndex, button), command);
		break;
	case ButtonState::downThisFrame:
		m_CommandsDown.emplace(std::make_pair(playerIndex, button), command);
		break;
	case ButtonState::releasedThisFrame:
		m_CommandsReleased.emplace(std::make_pair(playerIndex, button), command);
		break;
	}
}

void dae::XBox360Controller::RemoveInput(ControllerButton button, ButtonState state, int playerIndex)
{
	switch (state)
	{
	case ButtonState::pressed:
		m_CommandsPressed.erase(std::make_pair(playerIndex, button));
		break;
	case ButtonState::downThisFrame:
		m_CommandsDown.erase(std::make_pair(playerIndex, button));
		break;
	case ButtonState::releasedThisFrame:
		m_CommandsReleased.erase(std::make_pair(playerIndex, button));
		break;
	}
}

