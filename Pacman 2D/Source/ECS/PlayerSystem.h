#pragma once

#include "ECS/ECS.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Map/Map.h"
#include <glm/glm.hpp>
#include <dy/dyutils.h>
#include <ECS/SharedData.h>

class PlayerSystem: public System
{
public:
	inline static const glm::vec3 UP = { 0, -1, 0 };
	inline static const glm::vec3 DOWN = { 0, 1, 0 };
	inline static const glm::vec3 LEFT = { -1, 0, 0 };
	inline static const glm::vec3 RIGHT = { 1, 0, 0 };

	PlayerSystem();
	void Init(std::shared_ptr<Map> map);
	void Update(float dt);
	void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);
	void LoadViewMat(std::shared_ptr<Shader> shader, const glm::mat4& view);
	void LoadProjectMat(std::shared_ptr<Shader> shader, const glm::mat4& proj);
	void LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);
	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);
	void SetSharedData(std::shared_ptr<SharedData> sharedData);
	void HandlePlayerInput();
	void UpdatePacmanDirection();
	bool WillMove();
	void HandleMove();
	~PlayerSystem();
private:
	std::shared_ptr<Coordinator> coordinator;
	std::shared_ptr<SharedData> sharedData;

	uint VAO = 0;
	uint VBO = 0;
	uint EBO = 0;

	Entity curPlayer = 0;

	std::shared_ptr<Map> map;

	const glm::vec3 playerSpeed = glm::vec3(6.9, 6.9, 6.9);

	inline static const float VERTICES[] =
	{
		//pos					// tex
		-1, -1,					0 / 3.0f, 0 / 1.0f,
		 1, -1,					1 / 3.0f, 0 / 1.0f,
		 1,  1,					1 / 3.0f, 1 / 1.0f,
		-1,  1,					0 / 3.0f, 1 / 1.0f,
	};

	inline const static uint INDICES[] =
	{
		0, 3, 2,
		0, 2, 1,
	};

	void CreatePlayer(const glm::vec2& startPos);
	void ClearPlayer();
};

