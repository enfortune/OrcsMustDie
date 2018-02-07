#pragma once

//#include "nMath.h"

template<typename T> bool GraphDijkstra<T>::addLink(const T & start, const T & end, float cost, bool isBiDirection)
{
	if (cost <= 0.0f)
		return false;

	return Graph<T>::addLink(start, end, cost, isBiDirection);
}

template<typename T> bool GraphDijkstra<T>::updateLink(const T & start, const T & end, float cost, bool isBiDirection)
{
	if (cost <= 0.0f)
		return false;

	return Graph<T>::updateLink(start, end, cost, isBiDirection);
}

template<typename T> bool GraphDijkstra<T>::findPath(const T & start, const T & end,
	OUT std::vector<T> * pPathList, OUT float * pTotalCost)
{
	auto iterStart = linkTable_.find(start);
	if (iterStart == linkTable_.end())
		return false;

	if (start == end)
	{
		if (pPathList) pPathList->clear();
		if (pTotalCost) *pTotalCost = 0.0f;

		return true;
	}

	IF_STL_FIND_FAIL(linkTable_, end)
		return false;

	if (dirtyFlag_ == false)
	{
		IF_STL_FIND_SUCCESS(check, calcTable_, std::make_pair(start, end))
		{
			if (check->second.second == INFINITY_FLOAT)
				//if (nMath::isEqual(check->second.second, INFINITY_FLOAT))
				return false;

			if (pPathList) *pPathList = check->second.first;
			if (pTotalCost) *pTotalCost = check->second.second;

			return true;
		}
	}

	// === algorithm =======================================================================================================

	std::priority_queue<std::pair<float, T>, std::vector<std::pair<float, T> >, std::greater_equal<std::pair<float, T> > >
		priorityQueueMin;

	FOR_STL_ITERATOR_ALL(iterEnd, iterStart->second)
	{
		if (iterStart->first != iterEnd->first)
		{
			priorityQueueMin.emplace(std::make_pair(iterEnd->second, iterEnd->first));

			calcTable_.emplace(std::make_pair(iterStart->first, iterEnd->first),
				std::make_pair(std::vector<T>({iterStart->first, iterEnd->first}), iterEnd->second));
		}
	}

	std::map<T, bool> isCheckedMap;

	FOR_STL_ITERATOR_ALL(iterCheck, linkTable_)
	{
		isCheckedMap[iterCheck->first] = iterCheck->first == start ? true : false;

		if(start != iterCheck->first)
		{
			calcTable_.emplace(std::make_pair(start, iterCheck->first),
				std::make_pair(std::vector<T>(iterStart->first), INFINITY_FLOAT));
		}
	}

	// --- calculate -------------------------------------------------------------------------------------------------------

	while (priorityQueueMin.empty() == false)
	{
		// select current calc position
		auto priority = priorityQueueMin.top(); priorityQueueMin.pop();
		if (isCheckedMap[priority.second]) continue;

		auto iterStartToCurrent = calcTable_.find(std::make_pair(start, priority.second));
		if (priority.first > iterStartToCurrent->second.second) continue;

		// calc start to adjacent cost
		auto iterCurrent = linkTable_.find(priority.second);

		FOR_STL_ITERATOR_ALL(iterCurrentToAdjacent, iterCurrent->second)
		{
			if (iterCurrentToAdjacent->first == start) continue;
			if (isCheckedMap[iterCurrentToAdjacent->first]) continue;

			auto iterStartToAdjacent = calcTable_.find(std::make_pair(start, iterCurrentToAdjacent->first));

			if (iterStartToCurrent->second.second + iterCurrentToAdjacent->second < iterStartToAdjacent->second.second)
			{
				iterStartToAdjacent->second.first = iterStartToCurrent->second.first;
				iterStartToAdjacent->second.first.emplace_back(iterCurrentToAdjacent->first);

				iterStartToAdjacent->second.second = iterStartToCurrent->second.second + iterCurrentToAdjacent->second;

				priorityQueueMin.emplace(std::make_pair(iterStartToAdjacent->second.second, iterCurrentToAdjacent->first));
			}
		}

		isCheckedMap[priority.second] = true;
	}

	// =====================================================================================================================

	dirtyFlag_ = false;

	IF_STL_FIND_SUCCESS(result, calcTable_, std::make_pair(start, end))
	{
		if (result->second.second != INFINITY_FLOAT)
		{
			if (pPathList) *pPathList = result->second.first;
			if (pTotalCost) *pTotalCost = result->second.second;

			return true;
		}
	}

	return false;
}