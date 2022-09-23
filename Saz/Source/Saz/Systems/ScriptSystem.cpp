#include "SazPCH.h"
#include "ScriptSystem.h"
#include "Saz/Components/SceneComponent.h"
#include "Saz/Components/ScriptComponent.h"
#include "Saz/Core/GameTime.h"
#include "Saz/Scripting/ScriptEngine.h"

namespace ecs
{
	ecs::ScriptSystem::ScriptSystem()
	{

	}

	ecs::ScriptSystem::~ScriptSystem()
	{
		Saz::ScriptEngine::Shutdown();
	}

	void ecs::ScriptSystem::Init()
	{
		Saz::ScriptEngine::Init();
	}

	void ecs::ScriptSystem::Update(const Saz::GameTime& gameTime)
	{
		auto sceneStateView = m_World->GetAllEntitiesWith<component::SceneStateChangeRequestOneFrameComponent>();
		for (auto& entity : sceneStateView)
		{
			OnSceneStateChanged(entity);
		}

		if (!m_IsActive)
			return;

		auto view = m_World->GetAllEntitiesWith<component::ScriptComponent>();
		for (auto& entity : view)
		{
			Saz::ScriptEngine::OnUpdateEntity(m_World, entity, gameTime.GetDeltaTime());
		}
	}

	void ScriptSystem::OnSceneStateChanged(Entity entity)
	{
		auto& sceneStateChangedComp = m_World->GetComponent<component::SceneStateChangeRequestOneFrameComponent>(entity);

		using namespace Saz;

		if (sceneStateChangedComp.SceneState == SceneState::Play)
		{
			ScriptEngine::OnRuntimeStart(m_World);
			auto view = m_World->GetAllEntitiesWith<component::ScriptComponent>();
			for (auto& entity : view)
			{
				const auto& sc = m_World->GetComponent<component::ScriptComponent>(entity);
				if (ScriptEngine::EntityClassExists(sc.ClassName))
				{
					ScriptEngine::OnCreateEntity(m_World, entity);
				}
			}
			m_IsActive = true;
		}
		else if (sceneStateChangedComp.SceneState == SceneState::Editor || sceneStateChangedComp.SceneState == SceneState::ForceStop)
		{
			Saz::ScriptEngine::OnRuntimeStop(m_World);
			m_IsActive = false;
		}
	}
}