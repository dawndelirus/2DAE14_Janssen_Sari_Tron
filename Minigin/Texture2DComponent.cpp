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
	auto position = GetGameObject()->GetWorldPosition();
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
}

void dae::Texture2DComponent::SetTexture(const std::string& fileName)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

int dae::Texture2DComponent::GetWidth()
{
	return m_Texture->GetWidth();
}

int dae::Texture2DComponent::GetHeight()
{
	return m_Texture->GetHeight();
}
