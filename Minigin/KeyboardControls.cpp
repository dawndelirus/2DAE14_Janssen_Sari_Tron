#include "MiniginPCH.h"
#include "KeyboardControls.h"

class dae::KeyboardControls::KeyboardControlsImpl
{
public:
	KeyboardControlsImpl();
	~KeyboardControlsImpl();

	void Update();

	bool IsPressed(KeyboardKey key) const;
	bool IsDownThisFrame(KeyboardKey key) const;
	bool IsReleasedThisFrame(KeyboardKey key) const;

private:
	PBYTE m_CurrentState{};
	PBYTE m_PreviousState{};
	PBYTE m_KeyboardState0{};
	PBYTE m_KeyboardState1{};

	bool m_IsKeyboard0Active{false};
};

dae::KeyboardControls::KeyboardControls()
	: m_pImpl{new KeyboardControlsImpl()}
{
}

dae::KeyboardControls::~KeyboardControls()
{
	delete m_pImpl;
}

void dae::KeyboardControls::Update()
{
	m_pImpl->Update();

	for (auto& command : m_CommandsPressed)
	{
		if (IsPressed(command.first))
		{
			command.second->Execute();
		}
	}

	for (auto& command : m_CommandsDown)
	{
		if (IsDownThisFrame(command.first))
		{
			command.second->Execute();
		}
	}

	for (auto& command : m_CommandsReleased)
	{
		if (IsReleasedThisFrame(command.first))
		{
			command.second->Execute();
		}
	}
}

void dae::KeyboardControls::AddInput(KeyboardKey key, std::shared_ptr<Command> command, ButtonState state)
{
	switch (state)
	{
	case ButtonState::pressed:
		m_CommandsPressed.emplace(key, command);
		break;
	case ButtonState::downThisFrame:
		m_CommandsDown.emplace(key, command);
		break;
	case ButtonState::releasedThisFrame:
		m_CommandsReleased.emplace(key, command);
		break;
	}
}

void dae::KeyboardControls::RemoveInput(KeyboardKey key, ButtonState state)
{
	switch (state)
	{
	case ButtonState::pressed:
		m_CommandsPressed.erase(key);
		break;
	case ButtonState::downThisFrame:
		m_CommandsDown.erase(key);
		break;
	case ButtonState::releasedThisFrame:
		m_CommandsReleased.erase(key);
		break;
	}
}

bool dae::KeyboardControls::IsPressed(KeyboardKey key) const
{
	return m_pImpl->IsPressed(key);
}

bool dae::KeyboardControls::IsDownThisFrame(KeyboardKey key) const
{
	return m_pImpl->IsDownThisFrame(key);
}

bool dae::KeyboardControls::IsReleasedThisFrame(KeyboardKey key) const
{
	return m_pImpl->IsReleasedThisFrame(key);
}

dae::KeyboardControls::KeyboardControlsImpl::KeyboardControlsImpl()
	: m_KeyboardState0{ new BYTE[256] }
	, m_KeyboardState1{ new BYTE[256] }
{

	if (!GetKeyboardState(m_KeyboardState0) || !GetKeyboardState(m_KeyboardState1))
	{
		std::cout << "InputManager::Initialize >> Failed to GetKeyboardState.";
	}
}

dae::KeyboardControls::KeyboardControlsImpl::~KeyboardControlsImpl()
{
	delete[] m_KeyboardState0;
	delete[] m_KeyboardState1;
}

void dae::KeyboardControls::KeyboardControlsImpl::Update()
{
	BOOL getKeyboardResult;
	if (m_IsKeyboard0Active)
	{
		getKeyboardResult = GetKeyboardState(m_KeyboardState1);
		m_PreviousState = m_KeyboardState0;
		m_CurrentState = m_KeyboardState1;
	}
	else
	{
		getKeyboardResult = GetKeyboardState(m_KeyboardState0);
		m_PreviousState = m_KeyboardState1;
		m_CurrentState = m_KeyboardState0;
	}

	m_IsKeyboard0Active = !m_IsKeyboard0Active;

}

bool dae::KeyboardControls::KeyboardControlsImpl::IsPressed(KeyboardKey key) const
{
	return (m_CurrentState[static_cast<int>(key)] & 0xF0) != 0;
}

bool dae::KeyboardControls::KeyboardControlsImpl::IsDownThisFrame(KeyboardKey key) const
{
	bool currentFrameDown = (m_CurrentState[static_cast<int>(key)] & 0xF0) != 0;
	bool previousFrameDown = (m_PreviousState[static_cast<int>(key)] & 0xF0) != 0;

	return currentFrameDown && !previousFrameDown;
}

bool dae::KeyboardControls::KeyboardControlsImpl::IsReleasedThisFrame(KeyboardKey key) const
{
	bool currentFrameDown = (m_CurrentState[static_cast<int>(key)] & 0xF0) != 0;
	bool previousFrameDown = (m_PreviousState[static_cast<int>(key)] & 0xF0) != 0;

	return !currentFrameDown && previousFrameDown;
}
