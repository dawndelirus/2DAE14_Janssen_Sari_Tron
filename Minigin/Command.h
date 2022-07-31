#pragma once
#include "GameObject.h"
#include <memory>

namespace dae
{
	class Command
	{
	public:
		Command(std::shared_ptr<GameObject> pGameObject)
			: m_pGameObject{ pGameObject }
		{}

		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

	protected:
		std::shared_ptr<GameObject> GetGameObject() const
		{
			return m_pGameObject;
		}

	private:
		std::shared_ptr<GameObject> m_pGameObject;
	};
}
