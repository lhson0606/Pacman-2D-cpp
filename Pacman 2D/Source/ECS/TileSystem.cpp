#include "TileSystem.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "dy/Log.h"
#include "ECS/TileComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/TilePositionComponent.h"
#include "dy/dyutils.h"

void TileSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void TileSystem::InitMap(std::shared_ptr<Map> map, dy::Camera& cam)
{
	CleanUp();

	tileSize = map->GetTileSize();

	auto wall = map->data->getLayer(Map::WALL_LAYER);
	auto position = map->data->getLayer(Map::POSITION_LAYER);
	auto mapCenter = position->firstObj(Map::GHOST_HOUSE_INSIDE_POS_NAME);

	assert(wall != nullptr);
	assert(position != nullptr);
	assert(mapCenter != nullptr);

	//DyLogger::LogArg(DyLogger::LOG_INFO, "Map center: %d, %d", mapCenter->getPosition().x, mapCenter->getPosition().y);
	cam.translate(glm::vec3(mapCenter->getPosition().x / 8, mapCenter->getPosition().y / 8, 0));

	int w = map->GetWidth();
	int h = map->GetHeight();
	float textureWidth = 280;
	float textureHeight = 8;

	HandleWallLayer(wall->getTileData(), w, h);

	int count = 0;

	int tilesPerRow = textureWidth / 8;

	for (auto entity : entities)
	{
		auto& tile = coordinator->GetComponent<TileComponent>(entity);
		auto& transform = coordinator->GetComponent<TransformComponent>(entity);
		const uint tileId = tile.id;

		int x = (tileId - 1) % tilesPerRow * 8;
		int y = (tileId - 1) / tilesPerRow * 8;

		TileVertex bottomLeft;
		bottomLeft.x = transform.position.x;
		bottomLeft.y = transform.position.y + 1;
		bottomLeft.u = (float)x / textureWidth;
		bottomLeft.v = 1 - (float)y / textureHeight;

		TileVertex topLeft;
		topLeft.x = transform.position.x;
		topLeft.y = transform.position.y;
		topLeft.u = (float)x / textureWidth;
		topLeft.v = 1 - (float)(y + 8) / textureHeight;

		TileVertex bottomRight;
		bottomRight.x = transform.position.x + 1;
		bottomRight.y = transform.position.y + 1;
		bottomRight.u = (float)(x + 8) / textureWidth;
		bottomRight.v = 1 - (float)y / textureHeight;

		TileVertex topRight;
		topRight.x = transform.position.x + 1;
		topRight.y = transform.position.y;
		topRight.u = (float)(x + 8) / textureWidth;
		topRight.v = 1 - (float)(y + 8) / textureHeight;

		vertices.push_back(bottomLeft);
		vertices.push_back(topLeft);
		vertices.push_back(bottomRight);
		vertices.push_back(topRight);

		indices.push_back(1 + count * 4);
		indices.push_back(0 + count * 4);
		indices.push_back(2 + count * 4);
		indices.push_back(1 + count * 4);
		indices.push_back(2 + count * 4);
		indices.push_back(3 + count * 4);

		count++;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TileVertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TileVertex), (void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
}

void TileSystem::LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj)
{
	shader->Use();
	shader->SetMat4("projection", proj);
	shader->Stop();
}

void TileSystem::LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view)
{
	shader->Use();
	shader->SetMat4("view", view);
	shader->Stop();
}

void TileSystem::LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->Stop();
}

void TileSystem::Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	tex->Attach(0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->Stop();
}

void TileSystem::CleanUp()
{
	//make a copy of the entities
	auto temp = this->entities;
	for (auto entity : temp)
	{
		coordinator->DestroyEntity(entity);
	}

	//destroy VAO and VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void TileSystem::HandleWallLayer(TileData tileData, int w, int h)
{
	//DyLogger::LogArg(DyLogger::LOG_INFO, "Handling wall layer");

	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			auto tile = tileData[{x, y}];

			if (tile && tile->getId() != 0)
			{
				auto entity = coordinator->CreateEntity();
				coordinator->AddComponent<TileComponent>(entity, {tile->getId() });
				coordinator->AddComponent<TransformComponent>(entity, { x + 0.0f, y + 0.0f, 0.0f });
				coordinator->AddComponent<TilePositionComponent>(entity, { x, y });
			}
		}
	}
}