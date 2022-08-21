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
	PBYTE m_pCurrentState{};
	PBYTE m_pPreviousState{};
	PBYTE m_pKeyboardState0{};
	PBYTE m_pKeyboardState1{};

	bool m_IsKeyboard0Active{false};
};

dae::KeyboardControls::KeyboardControls()
	: m_pImpl{new KeyboardControlsImpl()}
{
}

dae::KeyboardControls::~KeyboardControls()
{
	m_CommandsReleased.clear();
	m_CommandsDown.clear();
	m_CommandsPressed.clear();
	delete m_pImpl;
}

void dae::KeyboardControls::Update()
{
	m_pImpl->Update();

	for (auto& command : m_CommandsPressed)
	{
		if (IsPressed(command.first))
		{
			command.second->Execute(1.f);
		}
	}

	for (auto& command : m_CommandsDown)
	{
		if (IsDownThisFrame(command.first))
		{
			command.second->Execute(1.f);
		}
	}

	for (auto& command : m_CommandsReleased)
	{
		if (IsReleasedThisFrame(command.first))
		{
			command.second->Execute(1.f);
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

void dae::KeyboardControls::ClearInput()
{
	m_CommandsPressed.clear();
	m_CommandsDown.clear();
	m_CommandsReleased.clear();
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
	: m_pKeyboardState0{ new BYTE[256] }
	, m_pKeyboardState1{ new BYTE[256] }
{

	if (!GetKeyboardState(m_pKeyboardState0) || !GetKeyboardState(m_pKeyboardState1))
	{
		std::cout << "InputManager::Initialize >> Failed to GetKeyboardState.";
	}
}

dae::KeyboardControls::KeyboardControlsImpl::~KeyboardControlsImpl()
{
	delete[] m_pKeyboardState0;
	delete[] m_pKeyboardState1;
}

void dae::KeyboardControls::KeyboardControlsImpl::Update()
{
	BOOL getKeyboardResult;
	if (m_IsKeyboard0Active)
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState1);
		m_pPreviousState = m_pKeyboardState0;
		m_pCurrentState = m_pKeyboardState1;
	}
	else
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState0);
		m_pPreviousState = m_pKeyboardState1;
		m_pCurrentState = m_pKeyboardState0;
	}

	m_IsKeyboard0Active = !m_IsKeyboard0Active;

}

bool dae::KeyboardControls::KeyboardControlsImpl::IsPressed(KeyboardKey key) const
{
	return (m_pCurrentState[static_cast<int>(key)] & 0xF0) != 0;
}

bool dae::KeyboardControls::KeyboardControlsImpl::IsDownThisFrame(KeyboardKey key) const
{
	bool currentFrameDown = (m_pCurrentState[static_cast<int>(key)] & 0xF0) != 0;
	bool previousFrameDown = (m_pPreviousState[static_cast<int>(key)] & 0xF0) != 0;

	return currentFrameDown && !previousFrameDown;
}

bool dae::KeyboardControls::KeyboardControlsImpl::IsReleasedThisFrame(KeyboardKey key) const
{
	bool currentFrameDown = (m_pCurrentState[static_cast<int>(key)] & 0xF0) != 0;
	bool previousFrameDown = (m_pPreviousState[static_cast<int>(key)] & 0xF0) != 0;

	return !currentFrameDown && previousFrameDown;
}
