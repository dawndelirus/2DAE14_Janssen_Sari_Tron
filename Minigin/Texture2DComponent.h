#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class Texture2DComponent final : public BaseComponent
	{
	public:
		Texture2DComponent(std::shared_ptr<GameObject> gameObject, const std::string& fileName);
		~Texture2DComponent() = default;

		virtual void Update() override {};
		virtual void Render() const override;

		void SetTexture(const std::string& fileName);

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};

}