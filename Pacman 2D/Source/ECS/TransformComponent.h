#pragma once

#include <glm/glm.hpp>

class TransformComponent
{
public:
	TransformComponent() = default;

	TransformComponent(float x, float y, float z);

	TransformComponent(const glm::vec3& position);

	void Translate(const glm::vec3& translation);

	void Scale(const glm::vec3& scale);

	glm::vec3 GetPosition();

	glm::vec3 GetRotation();

	glm::vec3 GetScale();

	void SetPosition(const glm::vec3& position);

	void SetRotation(const glm::vec3& rotation);

	void SetScale(const glm::vec3& scale);

	glm::mat4 GetModelMatrix();

	void UpdateModelMatrix();

private:
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
};
