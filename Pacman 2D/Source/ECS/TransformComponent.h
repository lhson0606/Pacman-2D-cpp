#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TransformComponent
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	TransformComponent() = default;

	TransformComponent(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}

	TransformComponent(const glm::vec3& position)
	{
		this->position = position;
	}
};

