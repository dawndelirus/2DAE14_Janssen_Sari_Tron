#pragma once
#include "BaseComponent.h"
#include <memory>

namespace dae
{
	class Command
	{
	public:
		Command(std::shared_ptr<BaseComponent> pGameObject)
			: m_Component{ pGameObject }
		{}

		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

	protected:
		std::shared_ptr<BaseComponent> GetComponent() const
		{
			return m_Component;
		}

	private:
		std::shared_ptr<BaseComponent> m_Component;
	};
}
