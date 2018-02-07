#pragma once
#include "Graph.h"

template<typename T, typename T2> std::vector<T> Graph<T, T2>::getNodeList() const noexcept
{
	std::vector<T> temp;
	temp.reserve(linkTable_.size());

	FOR_STL_ITERATOR_ALL(iter, linkTable_)
		temp.emplace_back(iter->first);

	return temp;
}

template<typename T, typename T2>
std::vector<typename Graph<T, T2>::LinkData> Graph<T, T2>::getLinkList() const noexcept
{
	std::vector<Graph<T, T2>::LinkData> temp;

	FOR_STL_ITERATOR_ALL(iterStart, linkTable_)
	{
		FOR_STL_ITERATOR_ALL(iterEnd, iterStart->second)
			temp.emplace_back(LinkData(iterStart->first, iterEnd->first, iterEnd->second));
	}

	temp.shrink_to_fit();

	return temp;
}

template<typename T, typename T2> bool Graph<T, T2>::addNode(const T & node)
{
	IF_STL_EMPLACE_SUCCESS(check, linkTable_, node, std::move(std::map<T, float>()))
		return true;

	return false;
}

template<typename T, typename T2> bool Graph<T, T2>::addLink(const T & start, const T & end, float cost, bool isBiDirection)
{
	IF_STL_FIND_SUCCESS(checkEnd, linkTable_, end)
	{
		IF_STL_FIND_SUCCESS(checkStart, linkTable_, start)
		{
			IF_STL_EMPLACE_SUCCESS(checkStartToEnd, checkStart->second, end, cost)
			{
				if (isBiDirection)
				{
					IF_STL_EMPLACE_SUCCESS(checkEndToStart, checkEnd->second, start, cost)
					{
						dirtyFlag_ = true;
						return true;
					}

					checkStart->second.erase(checkStartToEnd.first);
				}
				else
				{
					dirtyFlag_ = true;
					return true;
				}
			}
		}
	}

	return false;
}

template<typename T, typename T2> void Graph<T, T2>::removeNode(const T & node)
{
	FOR_STL_ITERATOR_ALL(iter, linkTable_)
		FOR_STL_ITERATOR_ERASE_ALL(iterErase, iter->second, iterErase->first == node);

	linkTable_.erase(node);

	dirtyFlag_ = true;
}

template<typename T, typename T2> void Graph<T, T2>::removeLink(const T & start, const T & end, bool isBiDirection)
{
	if (isBiDirection)
	{
		IF_STL_FIND_SUCCESS(check2, linkTable_, end)
			check2->second.erase(start);
	}

	IF_STL_FIND_SUCCESS(check, linkTable_, start)
		check->second.erase(end);

	dirtyFlag_ = true;
}

template<typename T, typename T2>
bool Graph<T, T2>::updateLink(const T & start, const T & end, float cost, bool isBiDirection)
{
	IF_STL_FIND_SUCCESS(checkStart, linkTable_, start)
	{
		IF_STL_FIND_SUCCESS(checkEnd, linkTable_, end)
		{
			IF_STL_FIND_SUCCESS(checkStartToEnd, checkStart->second, end)
			{
				if (isBiDirection)
				{
					IF_STL_FIND_SUCCESS(checkEndToStart, checkEnd->second, start)
					{
						checkEndToStart->second = cost;
						checkStartToEnd->second = cost;
						dirtyFlag_ = true;

						return true;
					}
				}
				else
				{
					checkStartToEnd->second = cost;
					dirtyFlag_ = true;

					return true;
				}
			}
		}
	}

	return false;
}

// --- console test -----------------------------------------------------------------------------------

#ifdef CONSOLE
template<typename T, typename T2> void Graph<T, T2>::printNode()
{
	FOR_STL_ITERATOR_ALL(iter, linkTable_)
		std::cout << iter->first << " ";

	std::cout << std::endl << std::endl;
}

template<typename T, typename T2> void Graph<T, T2>::printLink()
{
	FOR_STL_ITERATOR_ALL(iter, linkTable_)
	{
		std::cout << iter->first << " -> ";

		FOR_STL_ITERATOR_ALL(iter2, iter->second)
			std::cout << "(" << iter2->first << ", " << iter2->second << ") ";

		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
}
#endif