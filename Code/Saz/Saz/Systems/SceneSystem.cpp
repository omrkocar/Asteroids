#include "SazPCH.h"
#include "SceneSystem.h"

#include "Saz/Components/SceneComponent.h"
#include "Saz/Components/CameraComponent.h"
#include "Saz/Core/Application.h"
#include "Saz/Rendering/Renderer.h"
#include "Saz/SceneSerializer.h"

namespace ecs
{	
	constexpr char* strLastOpenScene = "LastOpenScene.txt";

	void SceneSystem::Init()
	{
		auto entity = m_World->CreateEntity();
		m_Scene = &m_World->AddComponent<component::LoadedSceneComponent>(entity);

		// #todo: Plug this back once you've figured out system ordering
		/*Saz::SceneSerializer serializer(*m_World);
		const String& scenePath = serializer.DeserializeLastOpenScene(strLastOpenScene);
		if (!scenePath.empty())
		{
			m_Scene->Path = scenePath;
			UpdateWindowName(scenePath);
			m_World->SortEntities();
		}
		else
		{
			NewScene();
		}*/
	}

	void SceneSystem::Destroy()
	{
		Saz::SceneSerializer serializer(*m_World);
		auto& sceneComp = m_World->GetSingleComponent<component::LoadedSceneComponent>();
		if (sceneComp.Path != "")
		{
			serializer.SerializeLastOpenScene(strLastOpenScene, sceneComp.Path);
		}
	}

	void SceneSystem::Update(const Saz::GameTime& gameTime)
	{
		const auto sceneStateView = m_World->GetAllEntitiesWith<component::SceneStateChangeRequestOneFrameComponent>();
		for (const auto& entity : sceneStateView)
		{
			OnSceneStateChangeRequest(entity);
		}

		const auto newSceneView = m_World->GetAllEntitiesWith<component::NewSceneRequestOneFrameComponent>();
		for (const auto& newSceneEntity : newSceneView)
		{
			auto& sceneRequestComp = m_World->GetComponent<component::NewSceneRequestOneFrameComponent>(newSceneEntity);
			NewScene();
		}

		const auto loadSceneView = m_World->GetAllEntitiesWith<component::LoadSceneRequestOneFrameComponent>();
		for (const auto& loadSceneEntity : loadSceneView)
		{
			auto& sceneRequestComp = m_World->GetComponent<component::LoadSceneRequestOneFrameComponent>(loadSceneEntity);
			LoadScene(sceneRequestComp.Path);
		}

		const auto saveSceneView = m_World->GetAllEntitiesWith<component::SaveSceneRequestOneFrameComponent>();
		for (const auto& saveSceneEntity : saveSceneView)
		{
			auto& sceneRequestComp = m_World->GetComponent<component::SaveSceneRequestOneFrameComponent>(saveSceneEntity);
			SaveScene(sceneRequestComp.Path);
		}
	}

	void SceneSystem::NewScene()
	{
		if (m_Scene->SceneState != SceneState::Editor)
		{
			m_Scene->SceneState = SceneState::Editor;
		}
		DestroySceneEntities();
		UpdateWindowName("");
	}

	void SceneSystem::LoadScene(const String& scenePath)
	{
		if (m_Scene->SceneState != SceneState::Editor)
		{
			m_Scene->SceneState = SceneState::Editor;
		}
		DestroySceneEntities();

		Saz::SceneSerializer serializer(*m_World);
		serializer.Deserialize(scenePath);
		
		UpdateWindowName(scenePath);
	}

	void SceneSystem::SaveScene(const String& scenePath)
	{
		Saz::SceneSerializer serializer(*m_World);
		serializer.Serialize(scenePath);

		UpdateWindowName(scenePath);
	}

	void SceneSystem::UpdateWindowName(const String& scenePath)
	{
		auto& scene = m_World->GetSingleComponent<component::LoadedSceneComponent>();
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

	void SceneSystem::DestroySceneEntities()
	{
		const auto sceneEntityView = m_World->m_Registry.view<component::SceneEntityComponent>();
		for (const auto& sceneEntity : sceneEntityView)
		{
			m_World->DestroyEntity(sceneEntity);
		}
	}

	void SceneSystem::OnSceneStateChangeRequest(Entity entity)
	{
		auto& sceneStateRequest = m_World->GetComponent<component::SceneStateChangeRequestOneFrameComponent>(entity);

		if (sceneStateRequest.SceneState == SceneState::Play)
		{
			Saz::SceneSerializer serializer(*m_World);
			serializer.Serialize("EditorScene.saz");

			m_Scene->SceneState = SceneState::Play;
		}
		else if (sceneStateRequest.SceneState == SceneState::Editor)
		{
			const auto sceneEntityView = m_World->m_Registry.view<component::SceneEntityComponent>();
			for (const auto& sceneEntity : sceneEntityView)
			{
				m_World->DestroyEntity(sceneEntity);
			}

			Saz::SceneSerializer serializer(*m_World);
			serializer.Deserialize("EditorScene.saz");
			m_Scene->SceneState = SceneState::Editor;
		}
		else if (sceneStateRequest.SceneState == SceneState::ForceStop)
		{
			m_Scene->SceneState = SceneState::Editor;
		}
	}
}
