#pragma once

//
// graph.h - Graph data structure.
//
// Written by Ryan Antkowiak
//

#include <cstdlib>
#include <initializer_list>
#include <map>
#include <queue>
#include <set>

namespace rda
{
	template<typename T>
	class graph
	{
	private:
		// maps a node to a set of out-going nodes
		std::map<T, std::set<T>> m_edges;

	public:
		// add an edge from node1 to node2
		void add_edge(const T& node1, const T& node2)
		{
			auto a = m_edges.find(node1);
			if (a == m_edges.end())
				m_edges[node1] = std::set<T>(std::initializer_list<T>({ node2 }));
			else
				a->second.insert(node2);
		}

		// retrieve the set of edges that go out from a node
		std::set<T> get_edges(const T& node)
		{
			auto a = m_edges.find(node);
			if (a == m_edges.end())
				return std::set<T>();
			return a->second;
		}

		// depth first search
		void dfs(const T& start, std::function<void(const T&)> visitFunc)
		{
			std::set<T> visited;
			dfs_r(start, visitFunc, visited);
		}

		// breadth first search
		void bfs(const T& start, std::function<void(const T&)> visitFunc)
		{
			std::set<T> enqueued;
			std::queue<T> q;

			q.push(start);
			enqueued.insert(start);

			while (!q.empty())
			{
				auto e = q.front();
				visitFunc(e);
				q.pop();

				auto iter = m_edges.find(e);

				if (iter != m_edges.end())
					for (auto& a : iter->second)
						if (enqueued.find(a) == enqueued.end())
						{
							q.push(a);
							enqueued.insert(a);
						}
			}
		}

	private:
		// recursive depth first search helper
		void dfs_r(const T& start, std::function<void(const T&)> visitFunc, std::set<T>& visited)
		{
			visited.insert(start);
			visitFunc(start);

			auto outgoing = m_edges.find(start);
			
			if (outgoing != m_edges.end())
			{
				for (auto& a : outgoing->second)
					if (visited.find(a) == visited.end())
						dfs_r(a, visitFunc, visited);
			}
		}

	}; // class graph

} // namespace rda
