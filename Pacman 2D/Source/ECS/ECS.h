#pragma once

//reference: https://austinmorlan.com/posts/entity_component_system/

#include <cstdint>
#include <bitset>
#include <memory>
#include <array>
#include <set>
#include <assert.h>
#include <queue>
#include <unordered_map>

#define MAX_ENTITIES 5000
#define MAX_COMPONENTS 32

using Entity = std::uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;//used to keep track of which components an entity has
using ComponentType = std::uint8_t;

class EntityManger
{
public:
	EntityManger()
	{
		livingEntityCount = 0;

		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			availableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = availableEntities.front();
		availableEntities.pop();
		++livingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		signatures[entity].reset();
		availableEntities.push(entity);
		--livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return signatures[entity];
	}

	~EntityManger()
	{}
private:
	std::queue<Entity> availableEntities;
	std::array<Signature, MAX_ENTITIES> signatures;
	std::uint32_t livingEntityCount;
};

class IComponentArray
{
public:
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual ~IComponentArray() = default;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
	ComponentArray() = default;

	void InsertData(Entity entity, T component)
	{
		assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");

		std::uint32_t newIndex = size;
		entityToIndexMap[entity] = newIndex;
		indexToEntityMap[newIndex] = entity;
		componentArray[newIndex] = component;
		++size;
	}

	void RemoveData(Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");

		std::uint32_t removedIndex = entityToIndexMap[entity];
		std::uint32_t lastIndex = size - 1;
		Entity entityToMove = indexToEntityMap[lastIndex];

		componentArray[removedIndex] = componentArray[lastIndex];
		entityToIndexMap[entityToMove] = removedIndex;
		indexToEntityMap[removedIndex] = entityToMove;

		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(lastIndex);

		--size;
	}

	T& GetData(Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");

		return componentArray[entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

	~ComponentArray()
	{}

private:
	std::array<T, MAX_ENTITIES> componentArray;
	std::unordered_map<Entity, std::uint32_t> entityToIndexMap;
	std::unordered_map<std::uint32_t, Entity> indexToEntityMap;
	std::uint32_t size;
};

class ComponentManager
{
public:
	ComponentManager()
	{
		componentTypeCount = 0;
	}

	template <typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once.");

		componentTypes.insert({ typeName, componentTypeCount });
		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		++componentTypeCount;
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : componentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

	template <typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return componentTypes[typeName];
	}

	template <typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template <typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template <typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}
private:
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;
	std::unordered_map<const char*, ComponentType> componentTypes;
	std::uint32_t componentTypeCount;

	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}
};

class System
{
public:
	std::set<Entity> entities;
	virtual ~System() = default;
};

class SystemManager
{
public:
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;

			system->entities.erase(entity);
		}
	}

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		systems.insert({ typeName, system });

		return system;
	}

	template <typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) != systems.end() && "System used before registered.");

		signatures.insert({ typeName, signature });
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->entities.insert(entity);
			}
			else
			{
				system->entities.erase(entity);
			}
		}
	}
private:
	std::unordered_map<const char*, Signature> signatures;
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
};

class Coordinator
{
public:
	Coordinator()
	{
		entityManager = std::make_unique<EntityManger>();
		componentManager = std::make_unique<ComponentManager>();
		systemManager = std::make_unique<SystemManager>();
	};

	/*
	+---------------------------+
	|          Entity			|
	+---------------------------+
	*/
	Entity CreateEntity()
	{
		return entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		entityManager->DestroyEntity(entity);
		componentManager->EntityDestroyed(entity);
		systemManager->EntityDestroyed(entity);
	}

	/*
	+---------------------------+
	|       Component			|
	+---------------------------+
	*/
	template <typename T>
	void RegisterComponent()
	{
		componentManager->RegisterComponent<T>();
	}

	template <typename T>
	ComponentType GetComponentType()
	{
		return componentManager->GetComponentType<T>();
	}

	template <typename T>
	void AddComponent(Entity entity, T component)
	{
		componentManager->AddComponent(entity, component);

		auto signature = entityManager->GetSignature(entity);
		signature.set(GetComponentType<T>(), true);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	void RemoveComponent(Entity entity)
	{
		componentManager->RemoveComponent<T>(entity);

		auto signature = entityManager->GetSignature(entity);
		signature.set(GetComponentType<T>(), false);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	T& GetComponent(Entity entity)
	{
		return componentManager->GetComponent<T>(entity);
	}

	/*
	+---------------------------+
	|          System			|
	+---------------------------+
	*/
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return systemManager->RegisterSystem<T>();
	}

	template <typename T>
	void SetSystemSignature(Signature signature)
	{
		systemManager->SetSignature<T>(signature);
	}

	~Coordinator() {};
private:
	std::unique_ptr<EntityManger> entityManager;
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<SystemManager> systemManager;
};