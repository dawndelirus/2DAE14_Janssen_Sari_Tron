#include "MiniginPCH.h"
#include "PlotIntComponent.h"
#include <chrono>

using namespace dae;
using namespace std::chrono;

PlotIntComponent::PlotIntComponent(std::shared_ptr<GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_Result{}
{
	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
	{
		m_Steps.push_back(static_cast<float>(stepsize));
	}
}

void PlotIntComponent::Plot(int size)
{
	m_Result.clear();

	std::vector<int> buffer{};
	buffer.resize(size, 1);

	int stepsize{ 1 };

	while (stepsize < 1025)
	{
		auto start = high_resolution_clock::now();
		for (size_t i = 0; i < buffer.size(); i += stepsize)
		{
			buffer[i] *= 2;
		}
		auto end = high_resolution_clock::now();

		float time = static_cast<float>(duration_cast<std::chrono::microseconds>(end - start).count());
		m_Result.push_back(time);

		stepsize *= 2;
	}
}

const std::vector<float>& dae::PlotIntComponent::GetResults() const
{
	return m_Result;
}

const std::vector<float>& dae::PlotIntComponent::GetSteps() const
{
	return m_Steps;
}
