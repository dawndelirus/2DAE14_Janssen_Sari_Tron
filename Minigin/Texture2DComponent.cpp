#include "MiniginPCH.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"


dae::Texture2DComponent::Texture2DComponent(std::shared_ptr<GameObject> gameObject, const std::string& fileName)
	: BaseComponent(gameObject)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::Texture2DComponent::Render() const
{
	auto position = m_SceneObject.lock()->GetWorldPosition();
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
}

void dae::Texture2DComponent::SetTexture(const std::string& fileName)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}
