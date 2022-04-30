#include "MiniginPCH.h"
#include "Clock.h"


float dae::Clock::m_DeltaTime = 0.f;

//float Time::GetDeltaTime() const
//{
//	return m_DeltaTime;
//}
//

void dae::Clock::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
}
