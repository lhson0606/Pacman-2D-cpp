#pragma once

#include <glm/matrix.hpp>
#include "ECS/ECS.h"
#include "Map/Map.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "ECS/SharedData.h"
#include "Algorithm/AStar.h"
#include "ECS/GhostComponent.h"

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

	inline static const glm::vec3 UP = { 0, -1, 0 };
	inline static const glm::vec3 DOWN = { 0, 1, 0 };
	inline static const glm::vec3 LEFT = { -1, 0, 0 };
	inline static const glm::vec3 RIGHT = { 1, 0, 0 };

	//Blinky (red), Pinky (pink), Inky (cyan), and Clyde (orange)

	void Init(std::shared_ptr<Map> map);

	void LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj);

	void LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view);

	void LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

	void LoadExtra(std::shared_ptr<Shader> shader);

	void Update(float dt);

	void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

	void CleanUp();

	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);

	void SetSharedData(std::shared_ptr<SharedData> sharedData);

	void CreateGhost(GhostType type, glm::vec3 startPos, glm::vec2 scatterPos);

	Entity GetGhost(GhostType type);

	void Move(GhostType type, glm::vec3 dir);

	void UpdateTargetPos(Entity ghost);

	void UpdateDebugTargetPos();

	void UpdateGhostDirIdx(Entity ghost);

	glm::vec3 GetDirectionVec(int dir);

	std::vector<int> GetValidDirs(Entity ghost, glm::vec2 pos);

	void UpdateGhostVelocity(Entity ghost);

	void HandleDebugInput();

	~GhostSystem();

private:
	std::shared_ptr<Coordinator> coordinator;
	uint VAO = 0;
	uint VBO = 0;
	uint EBO = 0;

	glm::vec3 ghostHouseInsidePos;
	glm::vec3 ghostHouseOutsidePos;

	Entity ghosts[GHOST_COUNT];

	void UpdateGhostUniforms(std::shared_ptr<Shader> shader);

	void UpdateDebugGhostPath();

	void UpdateGhostEyeDir(Entity ghost);

	std::shared_ptr<Map> map;

	dy::AStar astar = dy::AStar{ map };

	std::shared_ptr<SharedData> sharedData;

	const glm::vec3 ghostSpeed = glm::vec3(9.2f, 9.2f, 9.2f);

	double accumulatedTime = 0;

	inline static const float VERTICE[] =
	{
		//=====01=====
		//positions				//texture coords					//ghost id			//part id
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
};
