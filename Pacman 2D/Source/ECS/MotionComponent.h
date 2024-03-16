#pragma once

#include <glm/vec3.hpp>

class MotionComponent
{
public:
	MotionComponent() = default;
	MotionComponent(glm::vec3 velocity) :velocity(velocity) {}
	MotionComponent(float x, float y, float z) :velocity(glm::vec3(x, y, z)) {}
	glm::vec3 GetVelocity() { return velocity; }
	void SetVelocity(const glm::vec3& velocity) 
	{ 
		if (isnan(velocity.x) || isnan(velocity.y) || isnan(velocity.z))
		{
			assert(false);
		}
		this->velocity = velocity; 
	}
private:
	glm::vec3 velocity = glm::vec3(0.1643898f, 0.9863939238f, 0.f)*glm::vec3(5.0f);
};

