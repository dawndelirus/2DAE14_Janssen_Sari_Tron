#pragma once
#include <BaseComponent.h>

class LevelLayoutComponent;

class LevelVisualComponent : public dae::BaseComponent
{
public:
	LevelVisualComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout);
	~LevelVisualComponent() = default;

	void Update() override {}
	void Render() const override {}

	void CreateVisuals();
private:
	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;
};

