#include "AStar.h"

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

std::vector<glm::ivec2> dy::AStar::FindPath(const glm::ivec2& start, const glm::ivec2& end, dy::AStar::Direction initialDir)
{
	std::unordered_map<glm::ivec2, glm::ivec2> cameFrom;
	std::unordered_map<glm::ivec2, float> gScore;
	std::unordered_map<glm::ivec2, float> fScore;
	Comparator comparator = Comparator{ &fScore };
	std::priority_queue<glm::ivec2, std::vector<glm::ivec2>, decltype(comparator)> openSet(comparator);

	this->currentDir = initialDir;

	gScore[start] = 0.0f;
	fScore[start] = Heuristic(start, end);
	openSet.push(start);

	while (!openSet.empty())
	{
		auto current = openSet.top();

		if (current == end)
		{
			std::vector<glm::ivec2> path;
			while (current != start)
			{
				path.push_back(current);
				current = cameFrom[current];
			}
			return ReconstructPath(path);
		}

		openSet.pop();

		for (auto& neighbour : GetNeighbours(current))
		{
			float tentativeGScore = gScore[current] + Heuristic(current, neighbour);
			if (tentativeGScore < gScore[neighbour])
			{
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentativeGScore;
				fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, end);

				openSet.push(neighbour);
			}
		}
	}

	return std::vector<glm::ivec2>();
}

std::vector<glm::ivec2> dy::AStar::GetNeighbours(const glm::ivec2& pos)
{
	return std::vector<glm::ivec2>();
}

std::vector<glm::ivec2> dy::AStar::ReconstructPath(const std::vector<glm::ivec2>& list)
{
	return std::vector<glm::ivec2>();
}

float dy::AStar::Heuristic(const glm::ivec2& a, const glm::ivec2& b)
{
	return 0.0f;
}

std::shared_ptr<Map> dy::AStar::GetMap() const
{
	return std::shared_ptr<Map>();
}