#pragma once

#include "ECS/ECS.h"
#include "Render/shader.h"
#include <string>
#include <Map/Map.h>
#include <memory>
#include <vector>
#include "Render/Texture.h"
#include "Render/Camera.h"

class Coordinator;

struct TileVertex
{
	float x, y;
	float u, v;
};

class TileSystem :public System
{
	inline const static float TILE_MESH[] = {
		 0.5f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f,  0.5f
	};

	inline const static int INDICES[] = {
		0, 1, 3,
		1, 2, 3
	};

	using TileData = std::map<std::tuple<int, int>, tson::Tile*>;
public:
	TileSystem() = default;

	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);

	void InitMap(std::shared_ptr<Map> map, dy::Camera& cam);

	void LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj);

	void LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view);

	void LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

	void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex);

private:
	int tileSize = 0;
	std::shared_ptr<Coordinator> coordinator;

	void CleanUp();

	void HandleWallLayer(TileData tileData, int w, int h);

	uint VAO = 0;
	uint VBO = 0;
	uint EBO = 0;

	std::vector<TileVertex> vertices;
	std::vector<uint> indices;
};
