#include "DebugSystem.h"
#include <glad/glad.h>
#include "DebugPathComponent.h"
#include "dy/Log.h"
#include <glm/gtc/matrix_transform.hpp>

void DebugSystem::Init()
{
	//AddTestingPath();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//create large enough buffer to hold all the vertices
	std::vector temp(10000, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), temp.data(), GL_STATIC_DRAW);

	//for drawing target tiles
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void DebugSystem::AddTestingPath()
{
	for (int i = 0; i < 4; i++)
	{
		Entity pathEntity = coordinator->CreateEntity();

		std::vector<crushedpixel::Vec2> points{
			{ -0.25f + i * 0.25f, -0.5f  },
			{ -0.25f,  0.5f  },
			{  0.25f,  0.25f },
			{  0.0f,   0.0f  },
			{  0.25f, -0.25f + i * 0.25f },
			{ -0.4f - i * 0.25f,  -0.25f }
		};

		coordinator->AddComponent<DebugPathComponent>(pathEntity, { points, (float)i });
	}
}

void DebugSystem::Draw(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, std::shared_ptr<Texture> tex)
{
	PrepareTargetTile(targetTileShader);
	targetTileShader->Use();
	tex->Attach(0);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	targetTileShader->Stop();

	Prepare();

	if (!shouldDraw)
		return;

	pathShader->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
	pathShader->Stop();
}

void DebugSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void DebugSystem::Prepare()
{
	std::vector<float> vertices;

	for (Entity e : entities)
	{
		auto& path = coordinator->GetComponent<DebugPathComponent>(e);
		for (auto& point : path.vertices)
		{
			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(path.id);
		}
	}

	if (vertices.size() == 0)
	{
		vertexCount = 0;
		shouldDraw = false;
		return;
	}

	shouldDraw = true;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//load new data into the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

	if (glGetError() != GL_NO_ERROR)
	{
		DyLogger::LogArg(DyLogger::LOG_ERROR, "OpenGL error: %d", glGetError());
		//assert(false && "Error!");
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	vertexCount = vertices.size() / 3;
}

void DebugSystem::PrepareTargetTile(std::shared_ptr<Shader> shader)
{
	shader->Use();
	for (Entity e : entities)
	{
		auto& debugComponent = coordinator->GetComponent<DebugPathComponent>(e);
		glm::vec3 pos = debugComponent.targetPositions;
		int id = (int)debugComponent.id;
		shader->SetMat4("models[" + std::to_string(id) + "]", glm::translate(glm::mat4(1.0f), pos));
		//shader->SetMat4("models[" + std::to_string(count) + "]", glm::mat4(1.0f));
		assert(id < 4 && "Too many target tiles!");
	}
	shader->Stop();
}

void DebugSystem::LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->Stop();
}

void DebugSystem::LoadProjectMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 proj)
{
	pathShader->Use();
	pathShader->SetMat4("projection", proj);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetMat4("projection", proj);
	targetTileShader->Stop();
}

void DebugSystem::LoadViewMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 view)
{
	pathShader->Use();
	pathShader->SetMat4("view", view);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetMat4("view", view);
	targetTileShader->Stop();
}

void DebugSystem::LoadColors(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3)
{
	pathShader->Use();
	pathShader->SetVec3("colors[0]", color0);
	pathShader->SetVec3("colors[1]", color1);
	pathShader->SetVec3("colors[2]", color2);
	pathShader->SetVec3("colors[3]", color3);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetVec3("colors[0]", color0);
	targetTileShader->SetVec3("colors[1]", color1);
	targetTileShader->SetVec3("colors[2]", color2);
	targetTileShader->SetVec3("colors[3]", color3);
	targetTileShader->Stop();
}

DebugSystem::~DebugSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}