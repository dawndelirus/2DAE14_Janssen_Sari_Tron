#pragma once
#include "BaseComponent.h"
#include <memory>

namespace dae
{
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(std::shared_ptr<GameObject> gameObject, std::shared_ptr<TextComponent> textComponent);
		~FPSComponent() = default;

		void Update() override;
		void Render() const override {};

	private:
		float m_FPS;
		std::weak_ptr<TextComponent> m_TextComponent;
	};

}

