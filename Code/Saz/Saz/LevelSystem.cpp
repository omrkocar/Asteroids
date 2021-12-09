#include "SazPCH.h"
#include "LevelSystem.h"

#include "Saz/FileHelpers.h"
#include "Saz/InputComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/ResourceManager.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>
#include "Saz/Application.h"
#include "Saz/ResourceManager.h"

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
						int bp = 1;
					}
					if (componentType == "RenderComponent")
					{
						component::RenderComponent& renderComp = m_World->AddComponent<component::RenderComponent>(entity);

						if (component.HasMember("Texture"))
						{
							// temp hack. #todo: have ResourceManager handle this
							renderComp.texture = new raylib::Texture2D(component["Texture"].GetString());
						}
					}
					if (componentType == "CameraComponent")
					{
						component::CameraComponent& cameraComp = m_World->AddComponent<component::CameraComponent>(entity);
						cameraComp.camera2D = new raylib::Camera2D({ 320, 240 }, { 0,0 }, 0.f, 1.f);
							
						//cameraComp.camera2D->
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
