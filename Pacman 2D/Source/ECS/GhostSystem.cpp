#include "GhostSystem.h"
#include "ECS/GhostComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/TilePositionComponent.h"
#include "ECS/MotionComponent.h"
#include "ECS/DebugPathComponent.h"
#include "PlayerComponent.h"
#include <glm/vec3.hpp>
#include "dy/dyutils.h"
#include "dy/Log.h"
#include "dy/glutils.h"
#include <math.h>
#include "Algorithm/AStar.h"

void GhostSystem::Init(std::shared_ptr<Map> map)
{
	CleanUp();

	this->map = map;

	tson::Object* ghostStartPos[GHOST_COUNT];
	tson::Object* ghostScatterPos[GHOST_COUNT];

	auto posLayer = map->data->getLayer(Map::POSITION_LAYER);

	assert(posLayer != nullptr);

	//get ghost start position
	//#todo: probably should move this to ghost component
	ghostStartPos[PINKY] = posLayer->firstObj(Map::PINKY_START_POS_NAME);
	ghostStartPos[BLINKY] = posLayer->firstObj(Map::BLINKY_START_POS_NAME);
	ghostStartPos[INKY] = posLayer->firstObj(Map::INKY_START_POS_NAME);
	ghostStartPos[CLYDE] = posLayer->firstObj(Map::CLYDE_START_POS_NAME);

	ghostScatterPos[PINKY] = posLayer->firstObj(Map::PINKY_SCATTER_POS_NAME);
	ghostScatterPos[BLINKY] = posLayer->firstObj(Map::BLINKY_SCATTER_POS_NAME);
	ghostScatterPos[INKY] = posLayer->firstObj(Map::INKY_SCATTER_POS_NAME);
	ghostScatterPos[CLYDE] = posLayer->firstObj(Map::CLYDE_SCATTER_POS_NAME);

	assert(ghostStartPos[PINKY] != nullptr);
	assert(ghostStartPos[BLINKY] != nullptr);
	assert(ghostStartPos[INKY] != nullptr);
	assert(ghostStartPos[CLYDE] != nullptr);

	//get ghost house position
	auto ghostHouseInside = posLayer->firstObj(Map::GHOST_HOUSE_INSIDE_POS_NAME);
	auto ghostHouseOutside = posLayer->firstObj(Map::GHOST_HOUSE_OUTSIDE_POS_NAME);

	assert(ghostHouseInside != nullptr);
	assert(ghostHouseOutside != nullptr);

	CreateGhost(
		PINKY,
		{ ghostStartPos[PINKY]->getPosition().x / 8.f, ghostStartPos[PINKY]->getPosition().y / 8.f, 0 },
		{ ghostScatterPos[PINKY]->getPosition().x / 8.f, ghostScatterPos[PINKY]->getPosition().y / 8.f }
	);
	CreateGhost(
		BLINKY,
		{ ghostStartPos[BLINKY]->getPosition().x / 8.f, ghostStartPos[BLINKY]->getPosition().y / 8.f, 0 },
		{ ghostScatterPos[BLINKY]->getPosition().x / 8.f, ghostScatterPos[BLINKY]->getPosition().y / 8.f }
	);
	CreateGhost(
		INKY,
		{ ghostStartPos[INKY]->getPosition().x / 8.f, ghostStartPos[INKY]->getPosition().y / 8.f, 0 },
		{ ghostScatterPos[INKY]->getPosition().x / 8.f, ghostScatterPos[INKY]->getPosition().y / 8.f }
	);
	CreateGhost(
		CLYDE,
		{ ghostStartPos[CLYDE]->getPosition().x / 8.f, ghostStartPos[CLYDE]->getPosition().y / 8.f, 0 },
		{ ghostScatterPos[CLYDE]->getPosition().x / 8.f, ghostScatterPos[CLYDE]->getPosition().y / 8.f }
	);

	//create VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICE), VERTICE, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	//texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

	//ghost id
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));

	//part id
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindVertexArray(0);

	//testing
	dy::AStar astar{ map };
	auto path = astar.FindPath({ 1, 1 }, { 10, 10 }, dy::AStar::RIGHT);
}

void GhostSystem::LoadProjectMat(std::shared_ptr<Shader> shader, glm::mat4 proj)
{
	shader->Use();
	shader->SetMat4("projection", proj);
	shader->Stop();
}

void GhostSystem::LoadViewMat(std::shared_ptr<Shader> shader, glm::mat4 view)
{
	shader->Use();
	shader->SetMat4("view", view);
	shader->Stop();
}

void GhostSystem::LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->Stop();
}

void GhostSystem::LoadExtra(std::shared_ptr<Shader> shader)
{
	shader->Use();
	shader->SetFloat("texRowStride", 0.2f);
	shader->SetFloat("texColStride", 0.5f);
	shader->Stop();
}

void GhostSystem::Update(float dt)
{
	static double accumulatedTime = 0;
	float t = 0.5f * (1 + sin(accumulatedTime * 20));
	int temp = (t > 0.5f) ? 1 : 0;

	for (auto e : entities)
	{
		auto& ghostComponent = coordinator->GetComponent<GhostComponent>(e);
		ghostComponent.part[0] = temp;

		auto& transform = coordinator->GetComponent<TransformComponent>(e);

		UpdateTargetPos(e);

		if (ghostComponent.path.size() == 0)
		{
			continue;
		}

		glm::ivec2 targetTile = ghostComponent.path[0];

		//if the ghost is at the target tile, remove it from the path
		while (dy::isEqual(targetTile.x, transform.GetPosition().x) && dy::isEqual(targetTile.y, transform.GetPosition().y))
		{
			ghostComponent.path.erase(ghostComponent.path.begin());
			coordinator->GetComponent<MotionComponent>(e).SetVelocity(glm::vec3(0));
			transform.SetPosition(glm::vec3(targetTile.x, targetTile.y, 0));
			if (ghostComponent.path.size() == 0)
			{
				break;
			}
			targetTile = ghostComponent.path[0];
		}

		if (ghostComponent.path.size() == 0)
		{
			continue;
		}

		//set the direction to the next tile
		glm::vec3 dir = glm::normalize(glm::vec3(targetTile.x, targetTile.y, 0) - transform.GetPosition());
		//our normalizer can return nan!?

		if (isnan(dir.x) || isnan(dir.y) || isnan(dir.z))
		{
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Ghost direction is nan!");
		}

		auto& motion = coordinator->GetComponent<MotionComponent>(e);
		motion.SetVelocity(dir * ghostSpeed);
		UpdateGhostEyeDir(e, dir);
	}

	accumulatedTime += dt;

	//recalculate path
	UpdateDebugGhostPath();
	UpdateDebugTargetPos();
}

void GhostSystem::Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	tex->Attach(0);
	UpdateGhostUniforms(shader);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(INDICES) / sizeof(int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->Stop();
}

void GhostSystem::CleanUp()
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

void GhostSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void GhostSystem::SetSharedData(std::shared_ptr<SharedData> sharedData)
{
	this->sharedData = sharedData;
}

void GhostSystem::CreateGhost(GhostType type, glm::vec3 startPos, glm::vec2 scatterPos)
{
	auto newGhost = coordinator->CreateEntity();

	GhostComponent ghostComponent;
	ghostComponent.state = GhostComponent::State::IN_CAGE;

	switch (type)
	{
	case GhostSystem::BLINKY:
		ghostComponent.color = GhostComponent::BLINKY_COLOR;
		ghostComponent.state = GhostComponent::State::ALIVE;
		break;
	case GhostSystem::PINKY:
		ghostComponent.color = GhostComponent::PINKY_COLOR;
		break;
	case GhostSystem::INKY:
		ghostComponent.color = GhostComponent::INKY_COLOR;
		break;
	case GhostSystem::CLYDE:
		ghostComponent.color = GhostComponent::CLYDE_COLOR;
		break;
	default:
		assert(false && "Invalid ghost type");
		break;
	}

	ghostComponent.startPos = { startPos.x - TILE_OFFSET, startPos.y - TILE_OFFSET, 0 };
	ghostComponent.type = type;
	ghostComponent.scatterPos = scatterPos;
	ghostComponent.targetPos = scatterPos;
	ghostComponent.mode = GhostComponent::Mode::SCATTER;

	coordinator->AddComponent<GhostComponent>(newGhost, ghostComponent);
	coordinator->AddComponent<TransformComponent>(newGhost, TransformComponent{ ghostComponent.startPos });
	coordinator->AddComponent<TilePositionComponent>(newGhost, TilePositionComponent{ (int)startPos.x / 8, (int)startPos.y / 8 });
	coordinator->AddComponent<MotionComponent>(newGhost, MotionComponent{});
	coordinator->AddComponent<DebugPathComponent>(newGhost, DebugPathComponent{ (float)type });

	ghosts[type] = newGhost;
}

Entity GhostSystem::GetGhost(GhostType type)
{
	return ghosts[type];
}

void GhostSystem::Move(GhostType type, glm::vec3 dir)
{
	auto ghost = coordinator->GetComponent<TransformComponent>(ghosts[type]);
	auto& motion = coordinator->GetComponent<MotionComponent>(ghosts[type]);
	auto& ghostData = coordinator->GetComponent<GhostComponent>(ghosts[type]);
	motion.SetVelocity(dir * ghostSpeed);

	//change eye direction
	if (dir == glm::vec3(1, 0, 0))
	{
		ghostData.part[1] = 1;
	}
	else if (dir == glm::vec3(-1, 0, 0))
	{
		ghostData.part[1] = 3;
	}
	else if (dir == glm::vec3(0, 1, 0))
	{
		ghostData.part[1] = 2;
	}
	else if (dir == glm::vec3(0, -1, 0))
	{
		ghostData.part[1] = 0;
	}
}

void GhostSystem::UpdateTargetPos(Entity e)
{
	auto& ghostComponent = coordinator->GetComponent<GhostComponent>(e);
	auto& transform = coordinator->GetComponent<TransformComponent>(e);

	/*if (ghostComponent.mode == GhostComponent::Mode::SCATTER)
	{
		ghostComponent.targetPos = ghostComponent.scatterPos;
		return;
	}*/

	if (ghostComponent.mode == GhostComponent::Mode::FRIGHTENED)
	{
		assert(false && "Not implemented");
		return;
	}

	switch (ghostComponent.type)
	{
	case GhostSystem::BLINKY:
		//Blinky always chase pacman
		ghostComponent.targetPos = sharedData->GetPacmanPos();
		break;
	case GhostSystem::PINKY:
		switch (sharedData->GetPacmanDir())
		{
		case PlayerComponent::UP:
			//minus both x and y to simulate overflow
			ghostComponent.targetPos = sharedData->GetPacmanPos() + glm::vec2(-4, -4);
			break;
		case PlayerComponent::DOWN:
			ghostComponent.targetPos = sharedData->GetPacmanPos() + glm::vec2(0, +4);
			break;
		case PlayerComponent::LEFT:
			ghostComponent.targetPos = sharedData->GetPacmanPos() + glm::vec2(-4, 0);
			break;
		case::PlayerComponent::RIGHT:
			ghostComponent.targetPos = sharedData->GetPacmanPos() + glm::vec2(+4, 0);
			break;
			break;
		default:
			break;
		}
		break;
	case GhostSystem::INKY:
		//oposite of pacman direction to blinky
		auto pacmanPos = sharedData->GetPacmanPos();
		auto blinkyPos = coordinator->GetComponent<TransformComponent>(ghosts[BLINKY]).GetPosition();
		auto inkyTargetPos = pacmanPos + (pacmanPos - glm::vec2{ blinkyPos.x, blinkyPos.y });
		ghostComponent.targetPos = inkyTargetPos;
		break;
	case GhostSystem::CLYDE:
		//if the distance between clyde and pacman is less than 8 tiles, clyde will target scatter position,
		//otherwise it will target pacman
		ghostComponent.targetPos = glm::distance(
			transform.GetPosition(),
			glm::vec3{ sharedData->GetPacmanPos().x, sharedData->GetPacmanPos().y, 0 }
		)
			< 8 ? ghostComponent.scatterPos : sharedData->GetPacmanPos();

		break;
	default:
		break;
	}
}

void GhostSystem::UpdateDebugTargetPos()
{
	//Blinky
	auto& blinkyDebug = coordinator->GetComponent<DebugPathComponent>(ghosts[BLINKY]);
	auto& blinkyTargetPos = coordinator->GetComponent<GhostComponent>(ghosts[BLINKY]).targetPos;
	blinkyDebug.targetPositions = { blinkyTargetPos.x, blinkyTargetPos.y, 0 };

	//Pinky
	auto& pinkyDebug = coordinator->GetComponent<DebugPathComponent>(ghosts[PINKY]);
	auto& pinkyTargetPos = coordinator->GetComponent<GhostComponent>(ghosts[PINKY]).targetPos;
	pinkyDebug.targetPositions = { pinkyTargetPos.x, pinkyTargetPos.y, 0 };

	//Inky
	auto& inkyDebug = coordinator->GetComponent<DebugPathComponent>(ghosts[INKY]);
	auto& inkyTargetPos = coordinator->GetComponent<GhostComponent>(ghosts[INKY]).targetPos;
	inkyDebug.targetPositions = { inkyTargetPos.x, inkyTargetPos.y, 0 };

	//Clyde
	auto& clydeDebug = coordinator->GetComponent<DebugPathComponent>(ghosts[CLYDE]);
	auto& clydeTargetPos = coordinator->GetComponent<GhostComponent>(ghosts[CLYDE]).targetPos;
	clydeDebug.targetPositions = { clydeTargetPos.x, clydeTargetPos.y, 0 };
}

GhostSystem::~GhostSystem()
{
	//assume that if our system is deleted, our program is closing, no need to call CleanUp
}

void GhostSystem::UpdateGhostUniforms(std::shared_ptr<Shader> shader)
{
	int count = 0;

	for (int e : entities)
	{
		auto ghost = coordinator->GetComponent<GhostComponent>(e);
		auto transform = coordinator->GetComponent<TransformComponent>(e);

		std::string currentGhostData = "ghosts[" + std::to_string(count) + "].";

		shader->SetInt(currentGhostData + "body", ghost.part[0]);
		shader->SetInt(currentGhostData + "eye", ghost.part[1]);
		shader->SetVec3(currentGhostData + "color", ghost.color);
		shader->SetMat4(currentGhostData + "model", transform.GetModelMatrix());

		count++;
		assert(count <= GHOST_COUNT);
	}
}

template<typename T>
std::vector<T> removeConsecutiveDuplicates(const std::vector<T>& v)
{
	std::vector<T> result;
	result.reserve(v.size());
	std::unique_copy(v.begin(), v.end(), std::back_inserter(result), [](const crushedpixel::Vec2& a, const crushedpixel::Vec2& b) {return dy::isEqual(a.x, b.x) && dy::isEqual(a.y, b.y); });
	return result;
}

void GhostSystem::UpdateDebugGhostPath()
{
	for (Entity e : entities)
	{
		auto& transformComponent = coordinator->GetComponent<TransformComponent>(e);
		auto& debugPathComponent = coordinator->GetComponent<DebugPathComponent>(e);
		auto& ghostComponent = coordinator->GetComponent<GhostComponent>(e);

		debugPathComponent.ClearPath();

		/*if (ghostComponent.path.size() == 0)
		{
			continue;
		}*/

		std::vector<crushedpixel::Vec2> points;

		points.push_back(crushedpixel::Vec2(transformComponent.GetPosition().x, transformComponent.GetPosition().y));
		//#todo: this is for testing, remove this in the future

		for (const auto& v : ghostComponent.path)
		{
			points.push_back(crushedpixel::Vec2(v.x, v.y));
		}

		points.push_back({ ghostComponent.targetPos.x, ghostComponent.targetPos.y });

		points = removeConsecutiveDuplicates(points);

		debugPathComponent.SetPath(points);
	}
}

void GhostSystem::UpdateGhostEyeDir(Entity ghost, const glm::vec3 dir)
{
	auto& ghostComponent = coordinator->GetComponent<GhostComponent>(ghost);

	if (ghostComponent.mode == GhostComponent::Mode::FRIGHTENED)
	{
		ghostComponent.part[1] = 4;
	}
	else
	{
		float dpUp = glm::dot(dir, UP);
		float dpDown = glm::dot(dir, DOWN);
		float dpLeft = glm::dot(dir, LEFT);
		float dpRight = glm::dot(dir, RIGHT);

		float max = std::max({ dpUp, dpDown, dpLeft, dpRight });

		if (max == dpUp)
		{
			ghostComponent.part[1] = 0;
		}
		else if (max == dpDown)
		{
			ghostComponent.part[1] = 2;
		}
		else if (max == dpLeft)
		{
			ghostComponent.part[1] = 3;
		}
		else if (max == dpRight)
		{
			ghostComponent.part[1] = 1;
		}
	}
}