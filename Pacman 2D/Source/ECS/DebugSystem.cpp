#include "DebugSystem.h"
#include <glad/glad.h>
#include "DebugPathComponent.h"
#include "dy/Log.h"

void DebugSystem::Init()
{
	//AddTestingPath();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//create large enough buffer to hold all the vertices
	std::vector temp(10000, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), temp.data(), GL_STATIC_DRAW);
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

void DebugSystem::Draw(std::shared_ptr<Shader> shader)
{
	Prepare();

	if (!shouldDraw)
		return;

	shader->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
	shader->Stop();
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

void DebugSystem::LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj)
{
	shader->Use();
	shader->SetMat4("projection", proj);
	shader->Stop();
}

void DebugSystem::LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view)
{
	shader->Use();
	shader->SetMat4("view", view);
	shader->Stop();
}

void DebugSystem::LoadColors(std::shared_ptr<Shader> shader, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3)
{
	shader->Use();
	shader->SetVec3("colors[0]", color0);
	shader->SetVec3("colors[1]", color1);
	shader->SetVec3("colors[2]", color2);
	shader->SetVec3("colors[3]", color3);
	shader->Stop();
}

DebugSystem::~DebugSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}