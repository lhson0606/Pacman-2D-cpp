#pragma once

#include <glm/vec3.hpp>

class MotionComponent
{
public:
	MotionComponent() = default;
	MotionComponent(glm::vec3 velocity) :velocity(velocity) {}
	MotionComponent(float x, float y, float z) :velocity(glm::vec3(x, y, z)) {}
	glm::vec3 velocity = glm::vec3(0);
};

