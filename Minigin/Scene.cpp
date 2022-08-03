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
	m_DeletedObjects.clear();

	// In case another object gets added tot he scene during the update, it won't immedately update this frame as well.
	size_t objectAmount = m_Objects.size();

	for (size_t i = 0; i < objectAmount; ++i)
	{
		if (!m_Objects[i]->IsObjectDead())
		{
			m_Objects[i]->Update();
		}
		else
		{
			m_DeletedObjects.emplace(m_DeletedObjects.begin(), i);
		}
	}

	for (size_t i = m_DeletedObjects.size() - 1; i > 0; --i)
	{
		if (m_Objects[i]->IsObjectDead())
		{
			m_Objects[i] = m_Objects.back();
			m_Objects.pop_back();
		}
	}
	m_DeletedObjects.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

