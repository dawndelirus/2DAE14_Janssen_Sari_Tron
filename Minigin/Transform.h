#pragma once
namespace dae
{
	class Transform final
	{
	public:
		// TODO: TRANSFROM: Add rotation and scale
		Transform() = default;
		Transform(float x, float y, float z);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
	private:
		glm::vec3 m_Position;
	};
}
