#include "engine/pch.h"

#include "pathfinding.h"

#include "engine/map/map.h"
#include "engine/system/container.h"

#include <array>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>

typedef std::pair<double, raid::Position> PQElement;

bool operator<(const raid::Position& a, const raid::Position& b)
{
	return std::tie(a.GetX(), a.GetY()) < std::tie(b.GetX(), b.GetY());
}

bool operator>(const raid::Position& a, const raid::Position& b)
{
	return std::tie(a.GetX(), a.GetY()) > std::tie(b.GetX(), b.GetY());
}

namespace raid
{

void TilePath::Reset()
{
	m_Tiles.clear();
}

Tile* TilePath::GetDestination()
{
	if (!m_Tiles.empty())
	{
		return m_Tiles.back();
	}
	else
	{
		return nullptr;
	}
}

Position Pathfinding::TileDistanceBetweenTiles(const Position& first, const Position& second)
{
	return Position(first.GetX() - second.GetX(), first.GetY() - second.GetY());
}

double Pathfinding::DistanceBetweenTiles(const Position& pos1, const Position& pos2)
{
	// Calculate the Chebyshev distance using GetX() and GetY() methods
	int deltaX = std::abs(pos2.GetX() - pos1.GetX());
	int deltaY = std::abs(pos2.GetY() - pos1.GetY());
	return std::max(deltaX, deltaY);
}

struct PQElementCompare 
{
	bool operator()(const PQElement& a, const PQElement& b) const 
	{
		return a.first > b.first;
	}
};

struct PriorityQueue 
{
	std::priority_queue<PQElement, std::vector<PQElement>, PQElementCompare> elements;

	static int count;

	inline bool empty() const 
	{
		return elements.empty();
	}

	inline void put(Position item, double priority)
	{
		elements.emplace(priority, item);
		count++;
	}

	inline Position get()
	{
		Position best_item = elements.top().second;
		elements.pop();
		count--;
		return best_item;
	}
};

int PriorityQueue::count;

inline double heuristic(Position a, Position b) {
	return std::abs(a.GetX() - b.GetX()) + std::abs(a.GetY() - b.GetY());
}

void Pathfinding::AStarSearch(Map* graph, const Position& start, const Position& goal, std::unordered_map<Position, Position>& came_from, std::unordered_map<Position, double>& cost_so_far)
{
	PriorityQueue frontier;
	frontier.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) 
	{
		Position current = frontier.get();

		if (current == goal) {
			break;
		}

		std::vector<Position> results = graph->GetNeighbors(current);

		// Reverse search order on alternating tiles to reduce ugly path.
		if ((current.GetX() + current.GetY()) % 2 == 0)
		{
			std::reverse(results.begin(), results.end());
		}

		for (Position next : results)
		{
			double new_cost = cost_so_far[current] + graph->GetCost(current, next);
			if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) 
			{
				cost_so_far[next] = new_cost;
				double priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}

void Pathfinding::ReconstructPath(Map* graph, const Position& start, const Position& goal,
	std::unordered_map<Position, Position> came_from, TilePath& out_path)
{
	std::vector<Position> path;
	Position current = goal;

	if (came_from.find(goal) == came_from.end())
	{
		return;
	}

	while (current != start)
	{
		path.push_back(current);
		current = came_from[current];
	}

	path.push_back(start); // optional

	for (size_t i = path.size(); i --> 0; )
	{
		out_path.m_Tiles.push_back(graph->GetTile(path[i]));
	}
}

bool Pathfinding::FindClosestTile(Map* graph, const Position& start, const Position& goal, Position& out_result)
{
	std::set<Position> visitors;

	std::vector<Position> candidates;
	candidates.push_back(goal);

	while (candidates.size() > 0)
	{
		Position p = candidates.back();
		visitors.insert(p);
		candidates.pop_back();

		// Check if this tile is available?
		if (graph->CanOccupy(p))
		{
			out_result = p;
			return true;
		}

		// Add neighbours as candidates
		std::vector<Position> neighbours = graph->GetNeighbors(p);
		std::reverse(neighbours.begin(), neighbours.end());

		for (const Position& pos : neighbours)
		{
			// A valid tile we're not already after
			if (!SetContains(visitors, pos) && !VectorContains(candidates, pos) && graph->HasTile(pos))
			{
				candidates.push_back(pos);
			}
		}

		// Sort candidates so we check the best candidates first.
		// Since we pop from the back, best candidates go there.
		std::sort(candidates.begin(), candidates.end(), [&](const Position& a, const Position& b)
		{
			double a_to_goal = DistanceBetweenTiles(goal, a);
			double b_to_goal = DistanceBetweenTiles(goal, b);

			if (a_to_goal < b_to_goal)
				return false;
			else if (a_to_goal > b_to_goal)
				return true;

			return DistanceBetweenTiles(start, a) > DistanceBetweenTiles(start, b);
		});
	}

	return false;
}

bool Pathfinding::CanSwapTiles(const Position& first, const Position& second)
{
	return (std::abs(first.GetX() - second.GetX()) <= 1 && std::abs(first.GetY() - second.GetY()) <= 1);
}

} // namespace raid