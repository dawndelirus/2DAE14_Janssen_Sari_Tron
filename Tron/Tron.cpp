#pragma once
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <functional>

#include "Minigin.h"
#include "LevelManager.h"
#include "GameInfo.h"

void LoadGame()
{
	GameInfo::GetInstance().Initialize(2);

	auto& levelManager = LevelManager::GetInstance();

	levelManager.AddLevelPath("../Data/LevelLayout0.csv");
	levelManager.AddLevelPath("../Data/LevelLayout1.csv");
	levelManager.AddLevelPath("../Data/LevelLayout2.csv");

	levelManager.Initialize();
}

int main(int, char* [])
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}