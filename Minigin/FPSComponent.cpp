#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Time.h"
#include "TextComponent.h"

using namespace dae;

FPSComponent::FPSComponent(std::shared_ptr<GameObject> gameObject, std::shared_ptr<TextComponent> textComponent)
	: BaseComponent(gameObject)
	, m_TextComponent{textComponent}
	, m_FPS{}
{
}

void FPSComponent::Update()
{
	auto deltaTime = Time::GetInstance().GetDeltaTime();
	m_FPS = 1.f / deltaTime;
	m_TextComponent.lock()->SetText(std::to_string(static_cast<int>(m_FPS)));
}

