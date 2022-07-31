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

	glm::vec3 GetJoystickPosition(Joystick stick, int playerIndex) const;
	bool IsPressed(ControllerButton button, int playerIdx) const;
	bool IsDownThisFrame(ControllerButton button, int playerIdx) const;
	bool IsReleasedThisFrame(ControllerButton button, int playerIdx) const;

private:
	bool HasTimePassed();
	float CalculateMagnitude(int deadzone, float& normX, float& normY) const;

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

glm::vec3 dae::XBox360Controller::GetJoystickPosition(Joystick stick, int playerIndex) const
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
		if (abs(joystickPos.z) > 0.f)
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

glm::vec3 XBox360Controller::XBox360ControllerImpl::GetJoystickPosition(Joystick stick, int playerIndex) const
{
	glm::vec3 position{};

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

		float magnitude = CalculateMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, x, y);
		position = glm::vec3(x, y, magnitude);

		break;
	}
	case Joystick::RightStick:
	{
		float x = m_CurrentState[playerIndex].Gamepad.sThumbRX;
		float y = m_CurrentState[playerIndex].Gamepad.sThumbRY;

		float magnitude = CalculateMagnitude(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, x, y);

		position = glm::vec3(x, y, magnitude);
		break;
	}
	default:
		assert(false && "Joystick not recognised");
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

float XBox360Controller::XBox360ControllerImpl::CalculateMagnitude(int deadzone, float& x, float& y) const
{
	// https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput#the-xbox-controller

	//determine how far the controller is pushed
	float magnitude = sqrt(x * x + y * y);

	//determine the direction the controller is pushed
	x /= magnitude;
	y /= magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > deadzone)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767)
		{
			magnitude = 32767;
		}

		//adjust magnitude relative to the end of the dead zone
		magnitude -= deadzone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - deadzone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return normalizedMagnitude;
}
