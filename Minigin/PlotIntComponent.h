#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
    class PlotIntComponent : public BaseComponent
    {
	public:
		PlotIntComponent(std::shared_ptr<GameObject> gameObject);
		virtual ~PlotIntComponent() = default;

		void Update() override {};
		void Render() const override {};

		void Plot(int size);
		const std::vector<float>& GetResults() const;
		const std::vector<float>& GetSteps() const;
	private:
		std::vector<float> m_Result;
		std::vector<float> m_Steps;
    };
}
