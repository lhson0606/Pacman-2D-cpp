#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace dy
{
	class Camera
	{
	public:
		enum Movement
		{
			FORWARD,
			BACKWARD,
			RIGHT,
			LEFT,
			ASCEND,
			DESCEND
		};

		inline const static glm::vec3 POS = glm::vec3(0, 0, -10);
		inline const static glm::vec3 DIR = POS - glm::vec3(0, 0, 0);
		inline const static glm::vec3 UP = glm::vec3(0, 1, 0);
		inline const static float MOVE_SPEED = 0.0005f;
		inline const static float MOUSE_SENSITIVITY = 0.2f;
		inline const static float PITCH_MAX_CONSTRAINT = 89.0f;
		inline const static float PITCH_MIN_CONSTRAINT = -89.0f;
		inline const static float FOV = 1500;
		inline const static float PITCH = 0.0f;
		inline const static float YAW = 90;
		inline const static glm::vec3 WORLD_UP = glm::vec3(0, -1, 0);

		Camera(glm::vec3 pos = POS, glm::vec3 dir = DIR, glm::vec3 up = UP);

		void updateCamVectors();

		void processMouseInput(float dx, float dy, bool isPitchConstrained = true);

		void processZoom(float dy);

		void processMove(Movement moveDir);

		void rotate(float decree);

		void translate(glm::vec3 t);

		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec3 up;

		float pitch;
		float yaw;

		float fov;

		glm::mat4 view = glm::mat4(1);

		float moveSpeed;
		float mouseSensitivity;
	};
}