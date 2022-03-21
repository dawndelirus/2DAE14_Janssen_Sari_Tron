#pragma once
#include "GameObject.h"
#include <memory>

namespace dae
{
	class Command
	{
	public:
		Command(std::weak_ptr<GameObject> pGameObject)
			: m_pGameObject{ pGameObject }
		{
		}

		virtual ~Command() = default;
		virtual void Execute() = 0;

	protected:
		std::weak_ptr<GameObject> GetGameObject() const
		{
			return m_pGameObject;
		}

	private:
		std::weak_ptr<GameObject> m_pGameObject;
	};
}
