#include "MiniginPCH.h"
#include "XBox360Controller.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <array>
#include "Clock.h"

#pragma comment(lib, "xinput.lib")

using namespace dae;

class XBox360Controller::XBox360ControllerImpl
{
public:
	XBox360ControllerImpl();
	~XBox360ControllerImpl() = default;

	void Update();

	glm::vec2 GetJoystickPosition(Joystick stick, int playerIndex) const;
	bool IsPressed(ControllerButton button, int playerIdx) const;
	bool IsDownThisFrame(ControllerButton button, int playerIdx) const;
	bool IsReleasedThisFrame(ControllerButton button, int playerIdx) const;

private:
	bool HasTimePassed();

	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_CurrentState{};
	std::array<XINPUT_STATE, XUSER_MAX_COUNT> m_PreviousState{};

	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsPressedThisFrame{};
	std::array<WORD, XUSER_MAX_COUNT> m_ButtonsReleasedThisFrame{};

	std::array<bool, XUSER_MAX_COUNT> m_IsConnected{};

	float m_TotalTime{ 1.f };
	float m_CurrentTime{};
};

XBox360Controller::XBox360Controller()
{
	m_pImpl = new XBox360ControllerImpl();
}

XBox360Controller::~XBox360Controller()
{
	delete m_pImpl;
}

glm::vec2 dae::XBox360Controller::GetJoystickPosition(Joystick stick, int playerIndex) const
{
	return m_pImpl->GetJoystickPosition(stick, playerIndex);
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

	for (auto& command : m_CommandsJoystick)
	{
		auto joystickPos = GetJoystickPosition(command.first.second, command.first.first);
		if (joystickPos.x > 0.f || joystickPos.y > 0.f) // TODO: add deadzone
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

void dae::XBox360Controller::AddInput(Joystick stick, std::shared_ptr<Command> command, int playerIndex)
{
	m_CommandsJoystick.emplace(std::make_pair(playerIndex, stick), command);
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

void dae::XBox360Controller::RemoveInput(Joystick stick, int playerIndex)
{
	m_CommandsJoystick.erase(std::make_pair(playerIndex, stick));
}

XBox360Controller::XBox360ControllerImpl::XBox360ControllerImpl()
{
	{
		for (int i{}; i < XUSER_MAX_COUNT; ++i)
		{
			// reset input of all controllers
			ZeroMemory(&m_PreviousState[i], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[i], sizeof(XINPUT_STATE));

			// set all controllers to not connected
			m_IsConnected[i] = false;
		}
	}
}

void XBox360Controller::XBox360ControllerImpl::Update()
{
	bool hasTimePassed = HasTimePassed();

	for (int cIdx{}; cIdx < XUSER_MAX_COUNT; ++cIdx)
	{
		// check if controller is connected
		if (!hasTimePassed && !m_IsConnected[cIdx])
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
			m_IsConnected[cIdx] = true;

			// set buttons released and pressed
			CopyMemory(&m_CurrentState[cIdx], &state, sizeof(XINPUT_STATE));

			auto buttonChanges = m_CurrentState[cIdx].Gamepad.wButtons ^ m_PreviousState[cIdx].Gamepad.wButtons;
			m_ButtonsPressedThisFrame[cIdx] = buttonChanges & m_CurrentState[cIdx].Gamepad.wButtons;
			m_ButtonsReleasedThisFrame[cIdx] = buttonChanges & (~m_CurrentState[cIdx].Gamepad.wButtons);
		}
		else
		{
			m_IsConnected[cIdx] = false;

			// reset state of controller buttons
			ZeroMemory(&m_CurrentState[cIdx], sizeof(XINPUT_STATE));
			ZeroMemory(&m_PreviousState[cIdx], sizeof(XINPUT_STATE));
			continue;
		}
	}
}

glm::vec2 XBox360Controller::XBox360ControllerImpl::GetJoystickPosition(Joystick stick, int playerIndex) const
{
	glm::vec2 position{};

	if (playerIndex >= XUSER_MAX_COUNT)
	{
		return position;
	}
	else if (!m_IsConnected[playerIndex])
	{
		return position;
	}
	
	switch (stick)
	{
	case Joystick::LeftStick:
	{
		float x = m_CurrentState[playerIndex].Gamepad.sThumbLX;
		float y = m_CurrentState[playerIndex].Gamepad.sThumbLY;
		position = glm::vec2(x, y);
		break;
	}
	case Joystick::RightStick:
	{
		float x = m_CurrentState[playerIndex].Gamepad.sThumbRX;
		float y = m_CurrentState[playerIndex].Gamepad.sThumbRY;
		position = glm::vec2(x, y);
		break;
	}
	default:
		assert(true && "Joystick not recognised");
		break;
	}

	return position;
}

bool XBox360Controller::XBox360ControllerImpl::IsPressed(ControllerButton button, int playerIdx) const
{
	if (playerIdx >= XUSER_MAX_COUNT)
	{
		return false;
	}
	else if (!m_IsConnected[playerIdx])
	{
		return false;
	}
	return m_CurrentState[playerIdx].Gamepad.wButtons & static_cast<unsigned int>(button);
}

bool XBox360Controller::XBox360ControllerImpl::IsDownThisFrame(ControllerButton button, int playerIdx) const
{
	if (playerIdx >= XUSER_MAX_COUNT)
	{
		return false;
	}
	else if (!m_IsConnected[playerIdx])
	{
		return false;
	}
	return m_ButtonsPressedThisFrame[playerIdx] & static_cast<unsigned int>(button);
}

bool XBox360Controller::XBox360ControllerImpl::IsReleasedThisFrame(ControllerButton button, int playerIdx) const
{
	if (playerIdx >= XUSER_MAX_COUNT)
	{
		return false;
	}
	else if (!m_IsConnected[playerIdx])
	{
		return false;
	}
	return m_ButtonsReleasedThisFrame[playerIdx] & static_cast<unsigned int>(button);
}

bool XBox360Controller::XBox360ControllerImpl::HasTimePassed()
{
	if (m_CurrentTime >= m_TotalTime)
	{
		m_CurrentTime = 0.f;
		return true;
	}

	m_CurrentTime += dae::Clock::GetDeltaTime();
	return false;
}
