#include "SazPCH.h"
#include "SceneSystem.h"

#include "SceneComponent.h"
#include "SceneSerializer.h"
#include "Rendering/Renderer.h"
#include "Core/Application.h"

namespace ecs
{	
	constexpr char* strLastOpenScene = "LastOpenScene.txt";

	void SceneSystem::Init()
	{
		auto sceneEntity = m_World->CreateEntity();
		auto& sceneComp = m_World->AddComponent<component::LoadedSceneComponent>(sceneEntity);

		Saz::SceneSerializer serializer(*m_World);
		serializer.DeserializeLastOpenScene(strLastOpenScene);
	}

	void SceneSystem::PostInit()
	{
		
	}

	void SceneSystem::Destroy()
	{
		Saz::SceneSerializer serializer(*m_World);
		auto& sceneComp = m_World->GetSingleComponent<component::LoadedSceneComponent>();
		if (sceneComp.Path != "")
			serializer.SerializeLastOpenScene(strLastOpenScene, sceneComp.Path);
	}

	void SceneSystem::Update(const Saz::GameTime& gameTime)
	{
		const auto newSceneView = m_World->GetAllEntitiesWith<component::NewSceneRequestOneFrameComponent>();
		for (const auto& newSceneEntity : newSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::NewSceneRequestOneFrameComponent>(newSceneEntity);
			NewScene();
		}

		const auto loadSceneView = m_World->GetAllEntitiesWith<component::LoadSceneRequestOneFrameComponent>();
		for (const auto& loadSceneEntity : loadSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::LoadSceneRequestOneFrameComponent>(loadSceneEntity);
			LoadScene(sceneRequestComp.Path);
		}

		const auto saveSceneView = m_World->GetAllEntitiesWith<component::SaveSceneRequestOneFrameComponent>();
		for (const auto& saveSceneEntity : saveSceneView)
		{
			auto& sceneRequestComp = m_World->m_Registry.get<component::SaveSceneRequestOneFrameComponent>(saveSceneEntity);
			SaveScene(sceneRequestComp.Path);
		}
	}

	void SceneSystem::NewScene()
	{
		m_World->DestroyEntitesWith<component::SceneEntityComponent>();
		m_World->CreateMainCamera();
		UpdateWindowName("");
	}

	void SceneSystem::LoadScene(const String& scenePath)
	{
		const auto sceneEntityView = m_World->GetAllEntitiesWith<component::SceneEntityComponent>();
		for (const auto& sceneEntity : sceneEntityView)
		{
			m_World->DestroyEntity(sceneEntity);
		}

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
}
