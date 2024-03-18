#pragma once

#include "glm/glm.hpp"
#include <queue>
#include <unordered_map>
#include <vector>
#include "Map/Map.h"
#include <memory>
#include <bitset>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/hash.hpp"
/*
Note: my attemp to create custom hash function for glm::vec2, but I found a better solution

//referenced from https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
//our custom hash function for glm::vec2
template <>
struct std::hash<glm::vec2>
{
	//Note: I don't know if this hash function is good enough or not
	std::size_t operator()(const glm::vec2& k) const
	{
		using std::size_t;
		using std::hash;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		//return ((hash<int>()((int)k.x)
		//	^ (hash<int>()((int)k.y) << 1)) >> 1);

			//or this one!?
			//ref: https://stackoverflow.com/questions/13389631/whats-a-good-hash-function-for-struct-with-3-unsigned-chars-and-an-int-for-uno
	unsigned long long h = (int)k.x << 16 | (int)k.y;
	return std::hash<unsigned long long>()(h);
		}
	};

*/

//Note: In the end I found glm has built-in hash function :))). Yeah, whatever I spent like 1 hour to find the solution haha
//ref: https://stackoverflow.com/questions/9047612/glmivec2-as-key-in-unordered-map

namespace dy
{
	class Comparator
	{
	public:
		Comparator(std::unordered_map<glm::ivec2, float>* fScore)
		{
			this->fScore = fScore;
		}

		bool operator()(const glm::ivec2& a, const glm::ivec2& b)
		{
#ifdef DEBUG
			if (fScore->find(a) == fScore->end() || fScore->find(b) == fScore->end())
			{
				throw std::runtime_error("priority is not found");
			}
#endif // DEBUG			

			return fScore->find(a)->second > fScore->find(b)->second;
		}
	private:
		std::unordered_map<glm::ivec2, float>* fScore;
	};

	class AStar
	{
	public:
		enum Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		inline static const glm::ivec2 DIRECTIONS[] = {
			{0, -1}, //UP
			{0, 1}, //DOWN
			{-1, 0}, //LEFT
			{1, 0} //RIGHT
		};

		AStar(std::shared_ptr<Map> map);
		~AStar();

		std::vector<glm::ivec2> FindPath(const glm::ivec2& start, const glm::ivec2& end, int initialDir);

		std::vector<glm::ivec2> GetNeighbours(const glm::ivec2& pos);

		std::vector<glm::ivec2> ReconstructPath(const std::vector<glm::ivec2>& list);

		float Heuristic(const glm::vec2& a, const glm::vec2& b);

		std::shared_ptr<Map> GetMap() const;

	private:
		std::shared_ptr<Map> map;
		int currentDir = UP;
	};
}