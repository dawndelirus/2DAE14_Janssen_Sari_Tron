#pragma once
#include "BaseComponent.h"
#include <memory>
#include "PlotIntComponent.h"
#include "PlotGameObjectComponent.h"

namespace dae
{
	class ImGuiComponent final : public BaseComponent
	{
	public:
		ImGuiComponent(std::shared_ptr<GameObject> gameObject, std::shared_ptr<PlotIntComponent> intComp, std::shared_ptr<PlotGameObjectComponent> gameComp, SDL_Window* window);
		virtual ~ImGuiComponent();

		void Update() override;
		void Render() const override;
	private:
		void ImGuiWindow() const;

		SDL_Window* m_Window;
		std::weak_ptr<PlotIntComponent> m_IntComponent;
		std::weak_ptr<PlotGameObjectComponent> m_GameObjectComponent;
	};

}