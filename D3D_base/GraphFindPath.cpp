#include "stdafx.h"
#include "GraphFindPath.h"

void GraphFindPath::init()
{
	for (int i = 0; i < 5; ++i)
		graphDijkstra_.addNode(i);

	vertexList_.resize(5);
	vertexList_[0] = {30.0f, 0.0f, 40.0f};
	vertexList_[1] = {46.0f, 0.0f, 40.0f};
	vertexList_[2] = {46.0f, 0.0f, 20.0f};
	vertexList_[3] = {30.0f, 0.0f, 20.0f};
	vertexList_[4] = {30.0f, 0.0f, 10.0f};

	for (int i = 0; i < 4; ++i)
	{
		graphDijkstra_.addLink(i, i + 1,
			getDistance(vertexList_[i].x, vertexList_[i].z, vertexList_[i + 1].x, vertexList_[i + 1].z));
	}
}

bool GraphFindPath::findPath(int start, int end, OUT std::vector<D3DXVECTOR3>* pVertexList, OUT float * pTotalCost)
{
	std::vector<int> pathList;
	//if (graphDijkstra_.findPath(start, end, &pathList, pTotalCost))
	{
		if (pVertexList)
		{
			D3DXVECTOR3 vectorRandomMin {}, vectorRandomMax {};
			vectorRandomMin = {-5.0f, 0.0f, -5.0f};
			vectorRandomMax = {5.0f, 0.0f, 5.0f};

			for (int i = 0; i < static_cast<int> (pathList.size()); ++i)
			{
				pVertexList->emplace_back(vertexList_[pathList[i]]);

				D3DXVECTOR3 vectorRandom {};
				GetRandomVec3(&vectorRandom, &vectorRandomMin, &vectorRandomMax);

				(*pVertexList)[i] += vectorRandom;
			}
		}

		return true;
	}

	return false;
}