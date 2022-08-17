#include "LevelPathfindingComponent.h"
#include <vector>

LevelPathfindingComponent::LevelPathfindingComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout)
	: BaseComponent(gameObject)
	, m_LevelLayout(levelLayout)
{
}

std::vector<int> LevelPathfindingComponent::FindPath(int startIdx, int endIdx)
{
	std::vector<int> path{};
	std::vector<NodeRecord> openList{};
	std::vector<NodeRecord> closedList{};

	// 1. Create startRecord and add to open List to start while loop
	NodeRecord currentRecord{};
	currentRecord.idxCurrent = startIdx;
	currentRecord.idxPrevious = -1;
	currentRecord.fCost = GetHeuristicCost(startIdx, endIdx);

	openList.push_back(currentRecord);

	// 2. Continue searching for a connection that leads to the end node
	while (!openList.empty())
	{
		// 2.a Get the connection with lowest F score
		currentRecord = openList[0];
		for (const NodeRecord& recordFromList : openList)
		{
			if (recordFromList.fCost < currentRecord.fCost)
			{
				currentRecord = recordFromList;
			}
		}

		// 2.b Check if that connection leads to the end node
		if (currentRecord.idxCurrent == endIdx)
		{
			break;
		}

		// 2.c Get all the connections of the NodeRecord's node
		auto connectionList = m_LevelLayout.lock()->GetConnectingIndices(currentRecord.idxCurrent);
		for (int conectedIdx : connectionList)
		{
			// Calculate the total cost so far
			float gCost = 1 + currentRecord.gCost;

			// 2.d Check if any of them lead to a node already on the closed list

			bool isInClosed{ false };
			for (size_t i{}; i < closedList.size(); ++i)
			{
				if (closedList[i].idxCurrent == conectedIdx)
				{
					isInClosed = true;
					if (closedList[i].gCost > gCost)
					{
						const std::vector<NodeRecord>::iterator& it = closedList.begin() + i;
						closedList.erase(it);
						break;
					}
				}
			}

			if (isInClosed == true)
			{
				continue;
			}

			// 2.e If 2.d check failed, check if any of those connections lead to a node already on the open list
			bool isInOpen{ false };
			for (size_t i{}; i < openList.size(); ++i)
			{
				if (openList[i].idxCurrent == conectedIdx)
				{
					isInOpen = true;
					if (openList[i].gCost > gCost)
					{
						std::vector<NodeRecord>::iterator it = openList.begin() + i;
						openList.erase(it);
					}
					break;
				}
			}

			if (isInOpen == true)
			{
				continue;
			}

			// 2.f At this point any expensive connection should be removed. We create a new nodeRecord and add it to the openList
			NodeRecord newRecord{};
			newRecord.idxCurrent = conectedIdx;
			newRecord.idxPrevious = currentRecord.idxCurrent;
			newRecord.gCost = gCost;
			newRecord.fCost = gCost + GetHeuristicCost(conectedIdx, endIdx);
			openList.push_back(newRecord);
		}

		// 2.g Remove NodeRecord from the openList and add it to the closedList
		auto it = std::find(openList.begin(), openList.end(), currentRecord);
		assert(it != openList.end());
		openList.erase(it);
		closedList.push_back(currentRecord);
	}

	// 3. Reconstruct path from last connection to start node
	path.push_back(endIdx);

	while (currentRecord.idxCurrent != startIdx)
	{
		for (size_t i{}; i < closedList.size(); ++i)
		{
			if (closedList[i].idxCurrent == currentRecord.idxPrevious)
			{
				path.push_back(closedList[i].idxCurrent);
				currentRecord = closedList[i];
				break;
			}

			if (path.size() > m_LevelLayout.lock()->GetGridSize())
			{
				path.clear();
				path.push_back(startIdx);
				return path;
			}
		}
	}

	std::reverse(path.begin(), path.end());
	return path;
}

float LevelPathfindingComponent::GetHeuristicCost(int startIdx, int endIdx) const
{
	glm::vec2 toDestination = m_LevelLayout.lock()->GetGridCenter(endIdx) - m_LevelLayout.lock()->GetGridCenter(startIdx);
	return static_cast<float>(abs(toDestination.x + abs(toDestination.y)));
}
