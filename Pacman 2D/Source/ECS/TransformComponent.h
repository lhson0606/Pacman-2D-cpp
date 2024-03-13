#pragma once

#include <glm/glm.hpp>

class TransformComponent
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	TransformComponent() = default;

	TransformComponent(float x, float y, float z);

	TransformComponent(const glm::vec3& position);

	glm::mat4 GetModelMatrix();

private:
	glm::mat4 modelMatrix = glm::mat4(1.0f);
};
