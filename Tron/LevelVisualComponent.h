#pragma once
#include <BaseComponent.h>
#include <string>

class LevelLayoutComponent;

class LevelVisualComponent : public dae::BaseComponent
{
public:
	LevelVisualComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout, const std::string& wallFile, const std::string& voidFile, const std::string& pathFile, const std::string& teleportFile);
	~LevelVisualComponent() = default;

	void Update() override {}
	void Render() const override {}

private:
	void CreateVisuals(const std::string& wallFile, const std::string& voidFile, const std::string& pathFile, const std::string& teleportFile);
	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;
};

