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
static std::bitset<2000> isPopped{ 0 };
static std::bitset<2000> isVisited{ 0 };

std::vector<glm::ivec2> dy::AStar::FindPath(const glm::ivec2& start, const glm::ivec2& end, int initialDir)
{
	cameFrom.clear();
	gScore.clear();

	openSet = std::priority_queue<glm::ivec2, std::vector<glm::ivec2>, decltype(comparator)>{ comparator };

	//fScore must be cleared after openSet is cleared
	fScore.clear();
	isPopped.reset();
	isVisited.reset();

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
		isPopped[current.x*w + current.y] = 1;

		auto neighbours = GetNeighbours(current);

		for (auto& neighbour : neighbours)
		{
			float tentativeGScore = gScore[current] + Heuristic(current, neighbour);
			//auto test = gScore.find(neighbour)->second;
			//use gScore[neighbour] will create a new element if it does not exist, so be careful, use contains() instead
			//so yeah this code is pretty weird :))
			//if (!gScore.contains(neighbour))
			//{
			//	cameFrom[neighbour] = current;
			//	gScore[neighbour] = tentativeGScore;
			//	fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, end);

			//	//according to Wiki we should only add the neighbour to the open set if it is not in the open set
			//	//but I cannot find a way to check if the element is in std::priority_queue or not, it sucks ...
			//	if (isPopped[neighbour.x * w + neighbour.y] == 0)
			//	{
			//		openSet.push(neighbour);
			//		isPopped[neighbour.x * w + neighbour.y] = 0;
			//		count++;
			//	}				
			//}
			//else 
			if (tentativeGScore < gScore[neighbour] || !isVisited[neighbour.x * w + neighbour.y])
			{
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentativeGScore;
				isVisited[neighbour.x * w + neighbour.y] = 1;
				fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, end);

				if (isPopped[neighbour.x * w + neighbour.y] == 0)
				{
					openSet.emplace(neighbour);
					isPopped[neighbour.x * w + neighbour.y] = 0;
				}
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
		//the ghost cannot go back, so yeah ... not really A* algorithm
		/*if (i == DOWN && currentDir == DOWN)
		{
			continue;
		}
		else if (i == UP && currentDir == UP)
		{
			continue;
		}
		else if (i == RIGHT && currentDir == RIGHT)
		{
			continue;
		}
		else if (i == LEFT && currentDir == LEFT)
		{
			continue;
		}*/

		auto next = pos + DIRECTIONS[i];

		if (next.x < 0)
		{
			continue;
		}
		else if (next.x >= map->GetWidth())
		{
			continue;
		}

		if (next.y < 0)
		{
			continue;
		}
		else if (next.y >= map->GetHeight())
		{
			continue;
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