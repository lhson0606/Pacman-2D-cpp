#pragma once
#include "ECS/ECS.h"
#include "Render/Shader.h"
#include <memory>
#include "glm/vec3.hpp"

class DebugSystem : public System
{
public:
    DebugSystem() = default;
	void Init();
	void AddTestingPath();
	void Draw(std::shared_ptr<Shader> shader);
	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);
	void Prepare();
	void LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj);
	void LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view);
	void LoadColors(std::shared_ptr<Shader> shader, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3);
	~DebugSystem();

private:
	std::shared_ptr<Coordinator> coordinator;
	uint VAO = 0;
	uint VBO = 0;
	int vertexCount = 0;
};

