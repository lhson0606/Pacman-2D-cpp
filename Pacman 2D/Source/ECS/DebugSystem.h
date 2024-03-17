#pragma once
#include "ECS/ECS.h"
#include "Render/Shader.h"
#include <memory>
#include "glm/vec3.hpp"
#include "Render/Texture.h"

class DebugSystem : public System
{
public:
	DebugSystem() = default;
	void Init();
	void AddTestingPath();
	void Draw(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, std::shared_ptr<Texture> tex);
	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);
	void Prepare();
	void PrepareTargetTile(std::shared_ptr<Shader> shader);
	void LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);
	void LoadProjectMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 proj);
	void LoadViewMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 view);
	void LoadColors(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3);
	~DebugSystem();

private:
	std::shared_ptr<Coordinator> coordinator;

	//for drawing paths
	uint VAO = 0;
	uint VBO = 0;

	//for drawing target tiles
	uint VAO2 = 0;
	uint VBO2 = 0;
	uint EBO2 = 0;

	int vertexCount = 0;
	bool shouldDraw = false;

	inline static const float VERTICES[] =
	{
		//pos					// tex			//id
		-0.5f, -0.5f,					0, 0,			0,
		 0.5f, -0.5f,					1, 0,			0,
		 0.5f,  0.5f,					1, 1,			0,
		-0.5f,  0.5f,					0, 1,			0,
	//	 0.5f	0.5f
		-0.5f, -0.5f,					0, 0,			1,
		 0.5f, -0.5f,					1, 0,			1,
		 0.5f,  0.5f,					1, 1,			1,
		-0.5f,  0.5f,					0, 1,			1,
	//	 0.5f	0.5f
		-0.5f, -0.5f,					0, 0,			2,
		 0.5f, -0.5f,					1, 0,			2,
		 0.5f,  0.5f,					1, 1,			2,
		-0.5f,  0.5f,					0, 1,			2,
	//	 0.5f	0.5f
		-0.5f, -0.5f,					0, 0,			3,
		 0.5f, -0.5f,					1, 0,			3,
		 0.5f,  0.5f,					1, 1,			3,
		-0.5f,  0.5f,					0, 1,			3,
	};

	inline const static uint INDICES[] =
	{
		0, 2, 3,
		0, 1, 2,

		4, 6, 7,
		4, 5, 6,

		8, 10, 11,
		8, 9, 10,

		12, 14, 15,
		12, 13, 14,
	};
};
