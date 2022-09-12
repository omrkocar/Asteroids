#include "SazPCH.h"
#include "SceneSystem.h"

#include "SceneComponent.h"
#include "SceneSerializer.h"
#include "Rendering/Renderer.h"
#include "Core/Application.h"

namespace ecs
{	
	SceneSystem::SceneSystem()
	{

	}

	void SceneSystem::Init()
	{
		m_SceneEntity = m_World->CreateEntity();
		auto& sceneComp = m_World->AddComponent<component::LoadedSceneComponent>(m_SceneEntity);
	}

	void SceneSystem::PostInit()
	{
		
	}

	void SceneSystem::Destroy()
	{

	}

	void SceneSystem::Update(const Saz::GameTime& gameTime)
	{
		const auto newSceneView = m_World->GetAllEntitiesWith<component::NewSceneRequestOneFrameComponent>();
		for (const auto& newSceneEntity : newSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::NewSceneRequestOneFrameComponent>(newSceneEntity);
			NewScene(newSceneEntity);
		}

		const auto loadSceneView = m_World->GetAllEntitiesWith<component::LoadSceneRequestOneFrameComponent>();
		for (const auto& loadSceneEntity : loadSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::LoadSceneRequestOneFrameComponent>(loadSceneEntity);
			LoadScene(loadSceneEntity, sceneRequestComp.Path);
		}

		const auto saveSceneView = m_World->GetAllEntitiesWith<component::SaveSceneRequestOneFrameComponent>();
		for (const auto& saveSceneEntity : saveSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::SaveSceneRequestOneFrameComponent>(saveSceneEntity);
			SaveScene(saveSceneEntity, sceneRequestComp.Path);
		}

	}

	void SceneSystem::NewScene(Entity newSceneEntity)
	{
		m_World->DestroyEntitesWith<component::SceneEntityComponent>();
		m_World->DestroyEntity(newSceneEntity);
		m_World->CreateMainCamera();
		UpdateWindowName("");
	}

	void SceneSystem::LoadScene(Entity loadSceneEntity, const String& scenePath)
	{
		const auto sceneEntityView = m_World->GetAllEntitiesWith<component::SceneEntityComponent>();
		for (const auto& sceneEntity : sceneEntityView)
		{
			m_World->DestroyEntity(sceneEntity);
		}

		Saz::SceneSerializer serializer(*m_World);
		serializer.Deserialize(scenePath);
		
		UpdateWindowName(scenePath);

		if (m_World->IsAlive(loadSceneEntity))
			m_World->DestroyEntity(loadSceneEntity);
	}

	void SceneSystem::SaveScene(Entity saveSceneEntity, const String& scenePath)
	{
		Saz::SceneSerializer serializer(*m_World);
		serializer.Serialize(scenePath);

		UpdateWindowName(scenePath);

		m_World->DestroyEntity(saveSceneEntity);
	}

	void SceneSystem::UpdateWindowName(const String& scenePath)
	{
		auto& scene = m_World->GetComponent<component::LoadedSceneComponent>(m_SceneEntity);
		scene.Path = scenePath;
		String name = "Empty Scene";
		if (scenePath.empty() == false)
		{
			name = scenePath.substr(scenePath.find_last_of("/\\") + 1);
			name.erase(name.rfind('.'));
		}
		
		scene.Name = name;
		auto& window = Saz::Application::Get().GetWindow();
		window.SetTitle(String("Saz Editor") + " - " + scene.Name);
	}
}
