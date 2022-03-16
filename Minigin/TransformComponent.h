#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{

	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(std::shared_ptr<GameObject> gameObject, float x, float y);
		TransformComponent(std::shared_ptr<GameObject> gameObject, int x, int y);
		virtual ~TransformComponent();

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec2& position);
		void SetPosition(float x, float y);

		void Update() override {};
		void Render() const override {};

	private:
		dae::Transform m_Transform;
	};

}