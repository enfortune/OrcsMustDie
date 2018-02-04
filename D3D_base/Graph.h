#pragma once

#include "nCommon.h"

#ifndef CONSOLE
//#define CONSOLE
#endif

// T : node key, T2 : link cost
template <typename T, typename T2 = float>
class Graph
{
public:
	struct LinkData
	{
		T start {};
		T end {};
		T2 cost {};

		LinkData() = default;
		LinkData(T start, T end, T2 cost) : start(start), end(end), cost(cost) {}
	};

protected:
	std::map<T, std::map<T, float> > linkTable_;

	bool dirtyFlag_ {true};

public:
	Graph() = default;
	virtual ~Graph() = default;

	COPYABLE_DEFAULT(Graph);

public:
	GETTER_CONST(int, static_cast<int> (linkTable_.size()), NodeCount);
	std::vector<T> getNodeList() const noexcept;
	std::vector<LinkData> getLinkList() const noexcept;

public:
	bool addNode(const T & node);
	virtual bool addLink(const T & start, const T & end, float cost = 1.0f, bool isBiDirection = true);

	void removeNode(const T & node);
	void removeLink(const T & start, const T & end, bool isBiDirection = true);

	virtual bool updateLink(const T & start, const T & end, float cost = 1.0f, bool isBiDirection = true);

#ifdef CONSOLE
	void printNode();
	void printLink();
#endif
};

#include "Graph.hpp"

// === revision =============================================================================================

// 2018.01.26 : create
// 2018.01.27 : test
// 2018.02.02 : forward-referencing fail (cannot use template on virtual function), change to const T &

// === to do ================================================================================================

// Graph<T> -> Graph<T, T2> (float to all)