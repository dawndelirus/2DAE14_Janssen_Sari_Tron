#pragma once
#include <glm/vec3.hpp>

namespace dae
{
	class Transform final
	{
	public:
		// TODO: TRANSFROM: Add scale
		Transform() = default;
		Transform(const glm::vec3& pos);
		Transform(const glm::vec3& pos, float angleDeg);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		float GetRotation() const { return m_Angle; }
		void SetRotation(float angleDeg);

	private:
		glm::vec3 m_Position;
		float m_Angle;
	};
}
