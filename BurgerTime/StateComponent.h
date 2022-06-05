#pragma once
#include "BaseComponent.h"
#include "State.h"
#include <memory>

class StateComponent final : public dae::BaseComponent
{
public:
	StateComponent(std::weak_ptr<dae::GameObject> gameObject);
	virtual ~StateComponent();

	template <typename T>
	void AddState()
	{
		auto state = GetState<T>();
		if (state != nullptr)
		{
			m_pStates.emplace_back(std::make_unique<T>());
		}
	}

	//bool IsStateChangable();

protected:
	template <typename T>
	T* GetState() const
	{
		for (size_t i = 0; i < m_pStates.size(); ++i)
		{
			if (typeid(*m_pStates[i].get()) == typeid(T))
			{
				return m_pStates[i].get();
			}
		}
		return nullptr;
	}

private:
	std::vector<std::unique_ptr<State>> m_pStates;
	State* m_pCurrentState;
};
