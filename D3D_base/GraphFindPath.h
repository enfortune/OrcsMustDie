#pragma once

#include "GraphDijkstra.h"

class GraphFindPath
{
private:
	GraphDijkstra<int> graphDijkstra_;
	std::vector<D3DXVECTOR3> vertexList_;

public:
	void init();
	bool findPath(int start, int end, OUT std::vector<D3DXVECTOR3> * pVertexList = nullptr, OUT float * pTotalCost = nullptr);
};
