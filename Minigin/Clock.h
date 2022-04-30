#pragma once
#include "Singleton.h"

namespace dae
{

	class Clock final : public dae::Singleton<Clock>
	{
	public:
		static float GetDeltaTime()
		{
			return m_DeltaTime;
		}

		void SetDeltaTime(float deltaTime);

	private:
		static float m_DeltaTime;
	};

}