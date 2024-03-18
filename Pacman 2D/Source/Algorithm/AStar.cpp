#include "AStar.h"
#include "dy/Log.h"
#include <bitset>

//referenced from https://en.wikipedia.org/wiki/A*_search_algorithm

bool operator==(const glm::ivec2& a, const glm::ivec2& b)
{
	return a.x == b.x && a.y == b.y;
}

dy::AStar::AStar(std::shared_ptr<Map> map)
{
	this->map = map;
}

dy::AStar::~AStar()
{
}

static std::unordered_map<glm::ivec2, glm::ivec2> cameFrom;
static std::unordered_map<glm::ivec2, float> gScore;
static std::unordered_map<glm::ivec2, float> fScore;
static dy::Comparator comparator = dy::Comparator{ &fScore };
static std::priority_queue<glm::ivec2, std::vector<glm::ivec2>, decltype(comparator)> openSet(comparator);

std::vector<glm::ivec2> dy::AStar::FindPath(const glm::ivec2& start, const glm::ivec2& end, int initialDir)
{
	cameFrom.clear();
	gScore.clear();

	openSet = std::priority_queue<glm::ivec2, std::vector<glm::ivec2>, decltype(comparator)>{ comparator };

	//fScore must be cleared after openSet is cleared
	fScore.clear();

	this->currentDir = initialDir;

	gScore[start] = 0.0f;
	fScore[start] = Heuristic(start, end);
	openSet.emplace(start);

	int w = map->GetWidth();

	while (!openSet.empty())
	{
		auto current = openSet.top();

		if (current == end)
		{
			std::vector<glm::ivec2> path;
			while (current != start)
			{
				path.emplace_back(current);
				current = cameFrom[current];
			}
			return ReconstructPath(path);
		}

		openSet.pop();

		auto neighbours = GetNeighbours(current);

		for (auto& neighbour : neighbours)
		{
			float tentativeGScore = gScore[current] + Heuristic(current, neighbour);
			//auto test = gScore.find(neighbour)->second;
			//use gScore[neighbour] will create a new element if it does not exist, so be careful, use contains() instead
			//so yeah this code is pretty weird :))
			if (!gScore.contains(neighbour))
			{
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentativeGScore;
				fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, end);

				openSet.emplace(neighbour);
			}
			else if (tentativeGScore < gScore[neighbour])
			{
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentativeGScore;
				fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, end);

				openSet.emplace(neighbour);
			}
		}
	}

	return std::vector<glm::ivec2>();
}

std::vector<glm::ivec2> result;

std::vector<glm::ivec2> dy::AStar::GetNeighbours(const glm::ivec2& pos)
{
	result.clear();

	for (int i = 0; i < sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]); i++)
	{

		auto next = pos + DIRECTIONS[i];

		if (next.x < 0)
		{
			next = { map->GetWidth() - 1, next.y };
		}
		else if (next.x >= map->GetWidth())
		{
			next = { 0, next.y };
		}

		if (next.y < 0)
		{
			next = { next.x, map->GetHeight() - 1 };
		}
		else if (next.y >= map->GetHeight())
		{
			next = { next.x, 0 };
		}


		//#todo: the ghosts cannot go up in some specific tiles

		if (map->IsWall(next.x, next.y))
		{
			continue;
		}

		//if the next position is not a wall, we can add the next position
		result.emplace_back(next);
	}

	return result;
}

std::vector<glm::ivec2> dy::AStar::ReconstructPath(const std::vector<glm::ivec2>& list)
{
	//reverse the list
	std::vector<glm::ivec2> result;
	for (int i = list.size() - 1; i >= 0; i--)
	{
		result.push_back(list[i]);
	}
	return result;
}

float dy::AStar::Heuristic(const glm::vec2& a, const glm::vec2& b)
{
	return glm::distance(a, b);
}

std::shared_ptr<Map> dy::AStar::GetMap() const
{
	return this->map;
}