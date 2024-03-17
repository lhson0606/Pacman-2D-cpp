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

void TransformComponent::Scale(const glm::vec3& scale)
{
	this->scale = scale;
	modelMatrix = glm::scale(modelMatrix, scale);
}

glm::vec3 TransformComponent::GetPosition()
{
	return position;
}

glm::vec3 TransformComponent::GetRotation()
{
	return rotation;
}

glm::vec3 TransformComponent::GetScale()
{
	return scale;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
	this->position = position;
	UpdateModelMatrix();
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
	this->rotation = rotation;
	UpdateModelMatrix();
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
	UpdateModelMatrix();
}

glm::mat4 TransformComponent::GetModelMatrix()
{
	return modelMatrix;
}

void TransformComponent::UpdateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);
}