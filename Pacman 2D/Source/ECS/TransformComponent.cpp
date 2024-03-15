#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

TransformComponent::TransformComponent(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	modelMatrix = glm::translate(modelMatrix, position);
}

TransformComponent::TransformComponent(const glm::vec3& position)
{
	this->position = position;

	modelMatrix = glm::translate(modelMatrix, position);
}

void TransformComponent::Translate(const glm::vec3& translation)
{
	position += translation;

	modelMatrix = glm::translate(modelMatrix, translation);
}

glm::mat4 TransformComponent::GetModelMatrix()
{
	return modelMatrix;
}