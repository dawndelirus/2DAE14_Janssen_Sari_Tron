#include "MiniginPCH.h"
#include "PlotGameObjectComponent.h"
#include <chrono>

using namespace dae;
using namespace std::chrono;

PlotGameObjectComponent::PlotGameObjectComponent(std::shared_ptr<GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_Result{}
	, m_ResultAlt{}
{
	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
	{
		m_Steps.push_back(static_cast<float>(stepsize));
	}
}

void PlotGameObjectComponent::Plot(int size)
{
	m_Result.clear();
	
	std::vector<GameObject3D> buffer{};
	buffer.resize(size);

	int stepsize{ 1 };

	while (stepsize < 1025)
	{
		auto start = high_resolution_clock::now();
		for (size_t i = 0; i < buffer.size(); i += stepsize)
		{
			buffer[i].m_ID *= 2;
		}
		auto end = high_resolution_clock::now();

		float time = static_cast<float>(duration_cast<std::chrono::microseconds>(end - start).count());
		std::cout << stepsize << ";" << time << std::endl;
		m_Result.push_back(time);

		stepsize *= 2;
	}
}

const std::vector<float>& PlotGameObjectComponent::GetResults() const
{
	return m_Result;
}

void dae::PlotGameObjectComponent::PlotAlt(int size)
{
	m_ResultAlt.clear();

	std::vector<GameObject3DAlt> buffer{};
	buffer.resize(size);

	int stepsize{ 1 };

	while (stepsize < 1025)
	{
		auto start = high_resolution_clock::now();
		for (size_t i = 0; i < buffer.size(); i += stepsize)
		{
			buffer[i].m_ID *= 2;
		}
		auto end = high_resolution_clock::now();

		float time = static_cast<float>(duration_cast<std::chrono::microseconds>(end - start).count());
		std::cout << stepsize << ";" << time << std::endl;

		m_ResultAlt.push_back(time);

		stepsize *= 2;
	}
}

const std::vector<float>& PlotGameObjectComponent::GetResultsAlt() const
{
	return m_ResultAlt;
}

const std::vector<float>& dae::PlotGameObjectComponent::GetSteps() const
{
	return m_Steps;
}

