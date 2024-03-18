#include "GhostComponent.h"
#include <random>

std::vector<glm::ivec2> randomPathData =
{
	{1,1},
	{5,1},
	{0,0},
	{15,15},
	{17,5},
	{25,25},
	{20,32},
	{20,5},
	{15,5},
};

int ranInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

GhostComponent::GhostComponent()
{
	/*auto size = randomPathData.size();
	for (int i = 0; i < size; i++)
	{
		path.push_back(randomPathData[ranInt(0, size - 1)]);
	}*/
}