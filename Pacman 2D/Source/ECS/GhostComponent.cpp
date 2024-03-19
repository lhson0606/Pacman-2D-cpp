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

std::vector<glm::vec2> GhostComponent::GetTargetPos() const
{
	return targetPos;
}

void GhostComponent::ClearTargetPos()
{
	targetPos.clear();
}

void GhostComponent::PushbackTargetPos(const glm::vec2& pos)
{
	targetPos.push_back(pos);
}

void GhostComponent::EraseFirstTargetPos()
{
	targetPos.erase(targetPos.begin());
}

glm::vec2 GhostComponent::GetFirstTargetPos()
{
	return targetPos.at(0);
}

bool GhostComponent::IsTargetPosEmpty() const
{
	return targetPos.empty();
}
