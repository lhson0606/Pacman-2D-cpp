#pragma once

#include <glm/matrix.hpp>
#include "ECS/ECS.h"
#include "Map/Map.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

#define GHOST_COUNT 4

class GhostSystem :public System
{
public:
	enum GhostType
	{
		BLINKY,
		PINKY,
		INKY,
		CLYDE
	};

	//Blinky (red), Pinky (pink), Inky (cyan), and Clyde (orange)

	inline static const float VERTICE[] =
	{
		//=====01=====
		//positions				//texture coords		//ghost id			//part id
		-1, -1,					0 / 5.0f, 0 / 2.0f,					0,					0,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					0,					0,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					0,					0,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					0,					0,
		-1, -1,					0 / 5.0f, 0 / 2.0f,					0,					1,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					0,					1,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					0,					1,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					0,					1,
		//1	1				 /5.0f   /2.0f
		//1==1==02=====			 /5.0f   /2.0f
		//1po1itions				//5.0f/te/2.0fxture coords		//ghost id			//part id
		-1, -1,					0 / 5.0f, 0 / 2.0f,					1,					0,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					1,					0,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					1,					0,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					1,					0,
		-1, -1,					0 / 5.0f, 0 / 2.0f,					1,					1,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					1,					1,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					1,					1,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					1,					1,
		//1	1				 /5.0f   /2.0f
		//1==1==03=====			 /5.0f   /2.0f
		//1po1itions				//5.0f/te/2.0fxture coords		//ghost id			//part id
		-1, -1,					0 / 5.0f, 0 / 2.0f,					2,					0,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					2,					0,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					2,					0,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					2,					0,
		-1, -1,					0 / 5.0f, 0 / 2.0f,					2,					1,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					2,					1,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					2,					1,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					2,					1,
		//1	1				 /5.0f   /2.0f
		//1==1==04=====			 /5.0f   /2.0f
		//1po1itions				//5.0f/te/2.0fxture coords		//ghost id			//part id
		-1, -1,					0 / 5.0f, 0 / 2.0f,					3,					0,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					3,					0,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					3,					0,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					3,					0,
		-1, -1,					0 / 5.0f, 0 / 2.0f,					3,					1,
		 1, -1,					1 / 5.0f, 0 / 2.0f,					3,					1,
		 1,  1,					1 / 5.0f, 1 / 2.0f,					3,					1,
		-1,  1,					0 / 5.0f, 1 / 2.0f,					3,					1,
	};

	inline const static uint INDICES[] = {
		//===01===
		0, 3, 2,
		0, 2, 1,
		4, 7, 6,
		4, 6, 5,

		//===02===
		8, 11, 10,
		8, 10, 9,
		12, 15, 14,
		12, 14, 13,

		//===03===
		16, 19, 18,
		16, 18, 17,
		20, 23, 22,
		20, 22, 21,

		//===04===
		24, 27, 26,
		24, 26, 25,
		28, 31, 30,
		28, 30, 29,
	};

	void Init(std::shared_ptr<Map> map);

	void LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj);

	void LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view);

	void LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

	void LoadExtra(std::shared_ptr<Shader> shader);

	void Update(float dt);

	void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

	void CleanUp();

	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);

	void CreateGhost(GhostType type, glm::vec3 startPos);

	~GhostSystem();

private:
	std::shared_ptr<Coordinator> coordinator;
	uint VAO = 0;
	uint VBO = 0;
	uint EBO = 0;

	glm::vec3 ghostHouseInsidePos;
	glm::vec3 ghostHouseOutsidePos;
};
