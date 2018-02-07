#pragma once

#include "Graph.h"

//constexpr float FLOAT_INFINITY = 340282356779733642740000000000000000000.0f;
constexpr float INFINITY_FLOAT = 340282346630000000000000000000000000000.0f;

template <typename T>
class GraphDijkstra : public Graph<T>
{
private:
	std::map<std::pair<T, T>, std::pair<std::vector<T>, float> > calcTable_;

public:
	GraphDijkstra() = default;
	virtual ~GraphDijkstra() = default;

public:
	virtual bool addLink(const T & start, const T & end, float cost = 1.0f, bool isBiDirection = true) override;
	virtual bool updateLink(const T & start, const T & end, float cost = 1.0f, bool isBiDirection = true) override;

	bool findPath(const T & start, const T & end,
		OUT std::vector<T> * pPathList = nullptr, OUT float * pTotalCost = nullptr);
};

#include "GraphDijkstra.tpp"

// === revision ==================================================================================================

// before : create
// 2018.02.04 : test complete

// === to do =====================================================================================================


