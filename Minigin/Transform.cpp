#include "MiniginPCH.h"
#include "Transform.h"

dae::Transform::Transform(const glm::vec3& pos)
	: Transform(pos, 0.f)
{
}

dae::Transform::Transform(const glm::vec3& pos, float angleDeg)
	: m_Position{pos}
	, m_Angle{angleDeg}
{
}


void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void dae::Transform::SetRotation(float angleDeg)
{
	m_Angle = angleDeg;
}
