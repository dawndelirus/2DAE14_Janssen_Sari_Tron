#pragma once
#include <BaseComponent.h>
#include "LevelLayoutComponent.h"

class LevelPathfindingComponent : public dae::BaseComponent
{
public:
	LevelPathfindingComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout);

	void Update() override {};
	void Render() const override {};
	std::vector<int> FindPath(int startIdx, int endIdx);
	float GetHeuristicCost(int startIdx, int endIdx) const;

private:
	struct NodeRecord
	{
		int idxCurrent = -1;
		int idxPrevious = -1;
		float gCost = 0.f; // cost so far = accumulated g-costs of all the connections leading up to this one
		float fCost = 0.f; // estimatedTotalCost (= costSoFar + h-cost)

		bool operator==(const NodeRecord& other) const
		{
			return idxCurrent == other.idxCurrent
				&& idxPrevious == other.idxPrevious
				&& gCost == other.gCost
				&& fCost == other.fCost;
		};

		bool operator<(const NodeRecord& other) const
		{
			return fCost < other.fCost;
		};
	};

	std::weak_ptr<LevelLayoutComponent> m_LevelLayout;
};