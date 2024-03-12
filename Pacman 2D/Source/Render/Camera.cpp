#include "Render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "dy/dyutils.h"
#include <glm/gtc/quaternion.hpp>

dy::Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) :
	pos(pos), dir(dir), up(up),
	pitch(PITCH), yaw(YAW),
	moveSpeed(MOVE_SPEED), mouseSensitivity(mouseSensitivity),
	fov(FOV)
{
	dir = glm::normalize(dir);
	up = glm::normalize(up);
	updateCamVectors();
}

void dy::Camera::updateCamVectors()
{
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	dir = glm::normalize(dir);

	glm::vec3 right = glm::cross(dir, WORLD_UP);
	up = glm::normalize(glm::cross(right, dir));

	view = glm::lookAt(pos, pos + dir, up);
}

void dy::Camera::processMouseInput(float dx, float dy, bool isPitchConstrained)
{
	yaw += dx * MOUSE_SENSITIVITY;
	pitch += dy * MOUSE_SENSITIVITY;

	if (isPitchConstrained)
	{
		if (pitch > PITCH_MAX_CONSTRAINT)
		{
			pitch = PITCH_MAX_CONSTRAINT;
		}
		else if (pitch < PITCH_MIN_CONSTRAINT)
		{
			pitch = PITCH_MIN_CONSTRAINT;
		}
	}

	updateCamVectors();
}

void dy::Camera::processZoom(float dy)
{
	fov -= dy * 2.5;

	if (fov < 1.0f)
		fov = 1.0f;
	/*if (fov > 45.0f)
		fov = 45.0f;*/
}

void dy::Camera::processMove(Movement moveDir)
{
	switch (moveDir)
	{
	case dy::Camera::FORWARD:
		pos += dir * moveSpeed;
		break;
	case dy::Camera::BACKWARD:
		pos -= dir * moveSpeed;
		break;
	case dy::Camera::RIGHT:
	{
		glm::vec3 right = glm::normalize(glm::cross(dir, up));
		pos += right * moveSpeed;
		break;
	}
	case dy::Camera::LEFT:
	{
		glm::vec3 right = glm::normalize(glm::cross(dir, up));
		pos -= right * moveSpeed;
		break;
	}
	case dy::Camera::ASCEND:
		pos += WORLD_UP * moveSpeed;
		break;
	case dy::Camera::DESCEND:
		pos -= WORLD_UP * moveSpeed;
		break;
	default:
		break;
	}

	updateCamVectors();
}

void dy::Camera::rotate(float decree)
{
}

void dy::Camera::translate(glm::vec3 t)
{
	pos += t;
	updateCamVectors();
}
