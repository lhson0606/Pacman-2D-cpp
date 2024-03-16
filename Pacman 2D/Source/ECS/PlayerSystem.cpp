#include "PlayerSystem.h"
#include <glad/glad.h>
#include "ECS/PlayerComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/TilePositionComponent.h"
#include "ECS/MotionComponent.h"
#include "ECS/KeyboardComponent.h"
#include "ECS/KeyboardComponent.h"
#include "dy/Log.h"
#include "math.h"
#include "glm/vec3.hpp"
#include "dy/dyutils.h"

PlayerSystem::PlayerSystem()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)8);
	
	glBindVertexArray(0);
}

void PlayerSystem::Init(std::shared_ptr<Map> map)
{
	this->map = map;
	auto positionLayer = map->data->getLayer(Map::POSITION_LAYER);
	auto playerStartObj = positionLayer->firstObj(Map::PLAYER_START_POS_NAME);

	assert(playerStartObj != nullptr);

	float x = (float)playerStartObj->getPosition().x/map->GetTileSize();
	float y = (float)playerStartObj->getPosition().y/map->GetTileSize();

	glm::vec2 pos = glm::vec2(x, y);

	DyLogger::LogArg(DyLogger::LOG_INFO, "Player start position: (%f, %f)", pos.x, pos.y);

	CreatePlayer(pos);
}


void PlayerSystem::Update(float dt)
{
	static double accumulatedTime = 0.0;
	accumulatedTime += dt;
	
	float t = 1.5f * (1 + sin(accumulatedTime * 20));
	int idx = 0;

	if(coordinator->GetComponent<MotionComponent>(curPlayer).GetVelocity() != glm::vec3(0.0f))
	{
		float t = 1.5f * (1 + sin(accumulatedTime * 20));
		idx = (int)t;
	}
	
	coordinator->GetComponent<PlayerComponent>(curPlayer).SetAnimIdx(idx);

	HandlePlayerInput();
	HandleMove();

	//handle collision
	auto& motion = coordinator->GetComponent<MotionComponent>(curPlayer);
	auto pos = coordinator->GetComponent<TransformComponent>(curPlayer).GetPosition();
	auto dir = glm::normalize(motion.GetVelocity());

	if (isnan(dir.x) || isnan(dir.y))
	{
		dir = glm::vec3(0.0f);
	}

	if (dy::isInteger(pos.x) && dy::isInteger(pos.y))
	{
		if (map->IsWall((int)(pos.x + dir.x), (int)(pos.y + dir.y)))
		{
			motion.SetVelocity(glm::vec3(0.0f));
		}

		//DyLogger::LogArg(DyLogger::LOG_WARNING, "Player position: (%f, %f)", pos.x, pos.y);
	}

	UpdatePacmanDirection();
}

void PlayerSystem::Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	tex->Attach(0);
	shader->SetInt("texRow", coordinator->GetComponent<PlayerComponent>(curPlayer).GetAnimIdx());
	shader->SetMat4("model", coordinator->GetComponent<TransformComponent>(curPlayer).GetModelMatrix());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(INDICES) / sizeof(int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->Stop();
}

void PlayerSystem::LoadViewMat(std::shared_ptr<Shader> shader, const glm::mat4& view)
{
	shader->Use();
	shader->SetMat4("view", view);
	shader->Stop();
}

void PlayerSystem::LoadProjectMat(std::shared_ptr<Shader> shader, const glm::mat4& proj)
{
	shader->Use();
	shader->SetMat4("projection", proj);
	shader->Stop();
}

void PlayerSystem::LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	shader->SetInt("texture1", 0);
	//#todo: propably should not be hardcoded like this, use Sprite class to get the row count instead
	shader->SetFloat("rowStride", 1.0f / 3);
	shader->Stop();
}

void PlayerSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void PlayerSystem::SetSharedData(std::shared_ptr<SharedData> sharedData)
{
	this->sharedData = sharedData;
}

void PlayerSystem::HandlePlayerInput()
{
	auto& keyboard = coordinator->GetComponent<KeyboardComponent>(curPlayer);
	auto& motion = coordinator->GetComponent<MotionComponent>(curPlayer);

	if(keyboard.IsKeyPressed(GLFW_KEY_W))
	{
		coordinator->GetComponent<PlayerComponent>(curPlayer).SetInputDirection(PlayerComponent::UP);
		coordinator->GetComponent<KeyboardComponent>(curPlayer).Prepare();
	}

	if(keyboard.IsKeyPressed(GLFW_KEY_S))
	{
		coordinator->GetComponent<PlayerComponent>(curPlayer).SetInputDirection(PlayerComponent::DOWN);
		coordinator->GetComponent<KeyboardComponent>(curPlayer).Prepare();
	}

	if(keyboard.IsKeyPressed(GLFW_KEY_A))
	{
		coordinator->GetComponent<PlayerComponent>(curPlayer).SetInputDirection(PlayerComponent::LEFT);
		coordinator->GetComponent<KeyboardComponent>(curPlayer).Prepare();
	}

	if(keyboard.IsKeyPressed(GLFW_KEY_D))
	{
		coordinator->GetComponent<PlayerComponent>(curPlayer).SetInputDirection(PlayerComponent::RIGHT);
		coordinator->GetComponent<KeyboardComponent>(curPlayer).Prepare();
	}	
}

void PlayerSystem::UpdatePacmanDirection()
{
	auto& motion = coordinator->GetComponent<MotionComponent>(curPlayer);
	auto& transform = coordinator->GetComponent<TransformComponent>(curPlayer);
	auto dir = glm::normalize(motion.GetVelocity());
	
	if(motion.GetVelocity() == glm::vec3(0.0f))
	{
		return;
	}

	float dpUp = glm::dot(dir, UP);
	float dpDown = glm::dot(dir, DOWN);
	float dpLeft = glm::dot(dir, LEFT);
	float dpRight = glm::dot(dir, RIGHT);

	float max = std::max({ dpUp, dpDown, dpLeft, dpRight });

	if(max == dpUp)
	{
		transform.SetRotation({0.0f, 0.0f, -90.f});
	}
	else if(max == dpDown)
	{
		transform.SetRotation({ 0.0f, 0.0f, 90.f });
	}
	else if(max == dpLeft)
	{
		transform.SetRotation({ 0.0f, 0.0f, -180.f});
	}
	else if(max == dpRight)
	{
		transform.SetRotation({ 0.0f, 0.0f, 0.0f });
	}
}

bool PlayerSystem::WillMove()
{
	if(coordinator->GetComponent<PlayerComponent>(curPlayer).GetInputDirection() == PlayerComponent::NONE)
	{
		return false;
	}

	bool result = false;
	auto& motion = coordinator->GetComponent<MotionComponent>(curPlayer);
	auto curPos = coordinator->GetComponent<TransformComponent>(curPlayer).GetPosition();

	switch (coordinator->GetComponent<PlayerComponent>(curPlayer).GetInputDirection())
	{
	case PlayerComponent::UP:
		if (!dy::isInteger(curPos.x))
		{
			result = false;
		}
		else if (!dy::isInteger(curPos.y))
		{
			result = true;
		}
		else
		{
			result = !map->IsWall((int)curPos.x, (int)curPos.y - 1);
		}
		break;
	case PlayerComponent::DOWN:
		if (!dy::isInteger(curPos.x))
		{
			result = false;
		}
		else if (!dy::isInteger(curPos.y))
		{
			result = true;
		}
		else
		{
			result = !map->IsWall((int)curPos.x, (int)curPos.y + 1);
		}
		break;
	case PlayerComponent::LEFT:
		if (!dy::isInteger(curPos.y))
		{
			result = false;
		}
		else if (!dy::isInteger(curPos.x))
		{
			result = true;
		}
		else
		{
			result = !map->IsWall((int)curPos.x - 1, (int)curPos.y);
		}

		break;
	case PlayerComponent::RIGHT:
		if (!dy::isInteger(curPos.y))
		{
			result = false;
		}
		else if (!dy::isInteger(curPos.x))
		{
			result = true;
		}
		else
		{
			result = !map->IsWall((int)curPos.x + 1, (int)curPos.y);
		}
		break;
	case PlayerComponent::NONE:
		result = false;
		break;
	default:
		break;
	}

	return result;
}

void PlayerSystem::HandleMove()
{
	if (!WillMove())
	{
		return;
	}

	switch (coordinator->GetComponent<PlayerComponent>(curPlayer).GetInputDirection())
	{
		case PlayerComponent::UP:
			coordinator->GetComponent<MotionComponent>(curPlayer).SetVelocity(UP*playerSpeed);
			break;
		case PlayerComponent::DOWN:
			coordinator->GetComponent<MotionComponent>(curPlayer).SetVelocity(DOWN*playerSpeed);
			break;
		case PlayerComponent::LEFT:
			coordinator->GetComponent<MotionComponent>(curPlayer).SetVelocity(LEFT*playerSpeed);
			break;
		case PlayerComponent::RIGHT:
			coordinator->GetComponent<MotionComponent>(curPlayer).SetVelocity(RIGHT*playerSpeed);
			break;
		default:
			break;
	}

	coordinator->GetComponent<KeyboardComponent>(curPlayer).Prepare();
	coordinator->GetComponent<PlayerComponent>(curPlayer).SetInputDirection(PlayerComponent::NONE);
}

PlayerSystem::~PlayerSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void PlayerSystem::CreatePlayer(const glm::vec2& startPos)
{
	ClearPlayer();

	auto entity = coordinator->CreateEntity();

	coordinator->AddComponent<PlayerComponent>(entity, {1});
	coordinator->AddComponent<TransformComponent>(entity, {startPos.x - TILE_OFFSET, startPos.y - TILE_OFFSET, 0.0f});
	coordinator->AddComponent<TilePositionComponent>(entity, {(int)startPos.x, (int)startPos.y});
	coordinator->AddComponent<MotionComponent>(entity, {});
	coordinator->AddComponent<KeyboardComponent>(entity, {"pacman"});

	curPlayer = entity;
}

void PlayerSystem::ClearPlayer()
{
	assert(entities.size() < 1);
	auto temp = this->entities;

	for (auto entity : temp)
	{
		coordinator->DestroyEntity(entity);
	}
}
