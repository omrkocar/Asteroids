#include "SazPCH.h"
#include "LevelSystem.h"

#include "Saz/FileHelpers.h"
#include "Saz/InputComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/RenderSystem.h"
#include "Saz/ResourceManager.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>

namespace ecs
{	
	LevelSystem::LevelSystem(Saz::ResourceManager& resourceManager)
		: m_ResourceManager(resourceManager)
	{

	}

	bool LevelSystem::LoadFromFile(const String& filename)
	{
		m_World->DestroyAllEntities();

		FilePath relativePath = "D:/Dev/Saz/Data/Scenes/" + filename;
		const char* jsonSceneFile = Saz::file::LoadCompleteFile(relativePath.u8string().c_str(), nullptr);
		if (!jsonSceneFile)
		{
			SAZ_CORE_ERROR("File not found with path: {0}", relativePath.u8string());
			return false;
		}

		rapidjson::Document loadedScene;
		rapidjson::ParseResult ok = loadedScene.Parse(jsonSceneFile);
		delete[] jsonSceneFile;

		if (!ok)
			return false;

		for (rapidjson::Value& obj : loadedScene["Entities"].GetArray())
		{
			const ecs::Entity entity = m_World->CreateEntity();

			
			component::LevelComponent& levelComponent = m_World->AddComponent<component::LevelComponent>(entity);
			levelComponent.m_Name = relativePath.stem().string();
			levelComponent.m_Path = relativePath.string();
			
			component::NameComponent& nameComp = m_World->AddComponent<component::NameComponent>(entity);
			const String& entityName = obj["Name"].GetString();
			nameComp.m_Name = !entityName.empty() ? entityName : "InvalidName";
		
			// #todo: clean this up
			if (obj.HasMember("Components"))
			{
				for (rapidjson::Value& component : obj["Components"].GetArray())
				{
					const String& componentType = component["ComponentType"].GetString();
					if (componentType == "TransformComponent")
					{
						component::TransformComponent& transformComponent = m_World->AddComponent<component::TransformComponent>(entity);
						if (component.HasMember("Pos"))
							Saz::file::JSONLoadVec3(component, "Pos", &transformComponent.m_Position);
						if (component.HasMember("Scale"))
							Saz::file::JSONLoadVec3(component, "Scale", &transformComponent.m_Scale);
					}
					if (componentType == "MovementComponent")
					{
						component::MovementComponent& movementComponent = m_World->AddComponent<component::MovementComponent>(entity);
						const float speed = component["Speed"].GetFloat();
						movementComponent.m_Speed = speed;
					}
					if (componentType == "RenderComponent")
					{
						component::RenderComponent& renderComp = m_World->AddComponent<component::RenderComponent>(entity);
						Saz::file::JSONLoadVec3(component, "Color", &renderComp.color);

						if (component.HasMember("Type"))
						{
							/*const String& type = component["Type"].GetString();

							renderComp.m_Type = type;*/
						}
					}
					if (componentType == "CameraComponent")
					{
						m_World->AddComponent<component::CameraComponent>(entity);
					}
					if (componentType == "InputComponent")
					{
						m_World->AddComponent<component::InputComponent>(entity);
					}
				}
			}		
		}

		return true;
	}

}
