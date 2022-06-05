#pragma once
#include "BinToCharVec.h"
#include "Texture2DComponent.h"

glm::vec2 GetGridPosition(size_t idx, int width, int height);
void LoadLevel(dae::Scene* scene);
std::shared_ptr<dae::GameObject> LoadLevelVisuals(std::vector<char> output);
