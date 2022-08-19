#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include "glmWrapper.h"
#include "Observer.h"
#include <string>

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	class TextComponent final : public BaseComponent, public Observer
	{
	public:
		explicit TextComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& text, const std::shared_ptr<Font>& font, const glm::vec3& textColor);
		~TextComponent() = default;

		void Update() override;
		void Render() const override;

		void Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event);
		void SetText(const std::string& text);

	private:
		
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;

		glm::vec3 m_Color{};
	};
}