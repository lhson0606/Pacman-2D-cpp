#include "GhostComponent.h"

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

GhostComponent::GhostComponent()
{
	/*auto size = randomPathData.size();
	for (int i = 0; i < size; i++)
	{
		path.push_back(randomPathData[dy::ranInt(0, size - 1)]);
	}*/
}