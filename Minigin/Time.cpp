#include "MiniginPCH.h"
#include "Time.h"

float Time::m_DeltaTime = 0.f;

//float Time::GetDeltaTime() const
//{
//	return m_DeltaTime;
//}
//
void Time::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
}
