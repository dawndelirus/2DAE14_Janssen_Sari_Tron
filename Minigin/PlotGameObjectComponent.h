#pragma once
#include "BaseComponent.h"

namespace dae
{
	struct TransformMatrix
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		TransformMatrix m_Transform{};
		int m_ID{ 1 };
	};

	class GameObject3DAlt
	{
	public:
		TransformMatrix* m_Transform{ nullptr };
		int m_ID{ 1 };
	};

	class PlotGameObjectComponent : public BaseComponent
	{
	public:
		PlotGameObjectComponent(std::shared_ptr<GameObject> gameObject);
		virtual ~PlotGameObjectComponent() = default;

		void Update() override {};
		void Render() const override {};

		void Plot(int size);
		const std::vector<float>& GetResults() const;

		void PlotAlt(int size);
		const std::vector<float>& GetResultsAlt() const;

		const std::vector<float>& GetSteps() const;
	private:
		std::vector<float> m_Result;
		std::vector<float> m_ResultAlt;
		std::vector<float> m_Steps;
	};
}