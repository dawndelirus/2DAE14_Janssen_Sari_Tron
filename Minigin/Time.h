#pragma once
#include "Singleton.h"


class Time final : public dae::Singleton<Time>
{
public:
	Time() = default;
	~Time() = default;

	float GetDeltaTime() const;
	void SetDeltaTime(float deltaTime);

private:
	float m_DeltaTime = 0.f;
};

