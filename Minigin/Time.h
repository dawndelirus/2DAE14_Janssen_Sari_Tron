#pragma once
#include "Singleton.h"


class Time final : public dae::Singleton<Time>
{
public:
	static float GetDeltaTime()
	{
		return m_DeltaTime;
	}

	void SetDeltaTime(float deltaTime);

private:
	static float m_DeltaTime;
};

