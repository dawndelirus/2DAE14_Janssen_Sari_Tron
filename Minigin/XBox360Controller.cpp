#include "MiniginPCH.h"
#include "XBox360Controller.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <array>
#include <vector>
#include "Time.h"

#pragma comment(lib, "xinput.lib")

using namespace dae;

class XBox360Controller::XBox360ControllerImpl
{
public:
	XBox360ControllerImpl()
	{
		for (int i{}; i < XUSER_MAX_COUNT; ++i)
		{
			ZeroMemory(&m_PreviousState[i], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[i], sizeof(XINPUT_STATE));

			m_ControllerIdx.resize(XUSER_MAX_COUNT, -1);
			m_IsControllerConnected.resize(XUSER_MAX_COUNT, false);
		}
	}

	void Update()
	{
		bool hasTimePassed = HasTimePassed();

		for (int cIdx{}; cIdx < XUSER_MAX_COUNT; ++cIdx)
		{
			bool isConnected = m_IsControllerConnected[cIdx];
			if (!hasTimePassed && !isConnected)
			{
				continue;
			}

			CopyMemory(&m_PreviousState[cIdx], &m_CurrentState[cIdx], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[cIdx], sizeof(XINPUT_STATE));

			XINPUT_STATE state;
			DWORD result = XInputGetState(cIdx, &state);

			if (result == ERROR_SUCCESS)
			{
				if (!isConnected)
				{
					ConnectController(cIdx);
				}

				CopyMemory(&m_CurrentState[cIdx], &state, sizeof(XINPUT_STATE));

				auto buttonChanges = m_CurrentState[cIdx].Gamepad.wButtons ^ m_PreviousState[cIdx].Gamepad.wButtons;
				m_ButtonsPressedThisFrame[cIdx] = buttonChanges & m_CurrentState[cIdx].Gamepad.wButtons;
				m_ButtonsReleasedThisFrame[cIdx] = buttonChanges & (~m_CurrentState[cIdx].Gamepad.wButtons);

			}
			else
			{
				if (isConnected)
				{
					DisConnectController(cIdx);
				}

				ZeroMemory(&m_CurrentState[cIdx], sizeof(XINPUT_STATE));
				ZeroMemory(&m_PreviousState[cIdx], sizeof(XINPUT_STATE));
				continue;
			}
		}
	}

	bool IsPressed(ControllerButton button, int playerIdx) const
	{
		int cIdx = m_ControllerIdx[playerIdx];
		if (cIdx == -1)
		{
			return false;
		}
		return m_CurrentState[cIdx].Gamepad.wButtons & static_cast<unsigned int>(button);
	}

	bool IsDownThisFrame(ControllerButton button, int playerIdx) const
	{
		int cIdx = m_ControllerIdx[playerIdx];
		if (cIdx == -1)
		{
			return false;
		}
		return m_ButtonsPressedThisFrame[cIdx] & static_cast<unsigned int>(button);
	}

	bool IsReleasedThisFrame(ControllerButton button, int playerIdx) const
	{
		int cIdx = m_ControllerIdx[playerIdx];
		if (cIdx == -1)
		{
			return false;
		}
		return m_ButtonsReleasedThisFrame[cIdx] & static_cast<unsigned int>(button);
	}

private:
	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_CurrentState{};
	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_PreviousState{};

	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsPressedThisFrame{};
	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsReleasedThisFrame{};

	std::vector<int> m_ControllerIdx{};
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

		m_CurrentTime += Time::GetDeltaTime();
		return false;
	}

	void ConnectController(int cIdx)
	{
		auto it = std::find(m_ControllerIdx.begin(), m_ControllerIdx.end(), -1);
		m_ControllerIdx.at(it - m_ControllerIdx.begin()) = cIdx;
		m_IsControllerConnected[cIdx] = true;
	}

	void DisConnectController(int cIdx)
	{
		auto it = std::find(m_ControllerIdx.begin(), m_ControllerIdx.end(), cIdx);
		m_ControllerIdx.at(it - m_ControllerIdx.begin()) = -1;
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

