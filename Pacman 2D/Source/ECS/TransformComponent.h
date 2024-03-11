#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TransformComponent
{
public:
	glm::mat4 model = glm::mat4(1.0f);

	TransformComponent() = default;

	TransformComponent(float x, float y, float z)
	{
		model = glm::translate(model, glm::vec3(x, y, z));
	}

	TransformComponent(const glm::vec3& position)
	{
		model = glm::translate(model, position);
	}
	
	void Translate(const glm::vec3& translation)
	{
		model = glm::translate(model, translation);
	}

	void Rotate(float angle, const glm::vec3& axis)
	{
		model = glm::rotate(model, glm::radians(angle), axis);
	}

	void Scale(const glm::vec3& scale)
	{
		model = glm::scale(model, scale);
	}
};

