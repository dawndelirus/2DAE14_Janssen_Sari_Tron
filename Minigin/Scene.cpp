#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.emplace_back(object);
}

void Scene::Update()
{
	// In case another object gets added tot he scene during the update, it won't immedately update this frame as well.
	size_t objectAmount = m_Objects.size();

	for (size_t i = 0; i < objectAmount; ++i)
	{
		if (!m_Objects[i]->IsObjectDead())
		{
			m_Objects[i]->Update();
		}
	}

	for (size_t i = objectAmount - 1; i > 0; --i)
	{
		if (m_Objects[i]->IsObjectDead())
		{
			m_Objects[i] = m_Objects.back();
			m_Objects.pop_back();
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

