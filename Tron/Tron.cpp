#pragma once
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <functional>
#include <memory>

#include "Minigin.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "BaseSceneManager.h"

void LoadGame()
{
	auto scene = dae::ServiceLocator::GetSceneManager().CreateScene("Demo");
	auto soBg = std::make_shared<dae::GameObject>();
	auto textureBg = std::make_shared<dae::Texture2DComponent>(soBg, "background.jpg");
	soBg->AddComponent(textureBg);
	scene->Add(soBg);

	dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);
}

int main(int, char* [])
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}