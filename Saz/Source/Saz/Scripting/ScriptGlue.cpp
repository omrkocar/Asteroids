#include "SazPCH.h"
#include "ScriptGlue.h"

#include "Saz/Core/EntityWorld.h"
#include "Saz/Scripting/ScriptEngine.h"
#include "Components/TransformComponent.h"
#include "Saz/Core/KeyCodes.h"
#include "Components/InputComponent.h"
#include "Components/ComponentGroup.h"
#include "Saz/Components/CameraComponent.h"
#include "Saz/Components/PhysicsComponents.h"
#include "Saz/Components/RenderComponents.h"
#include "Saz/Components/ScriptComponent.h"

#include <box2d/b2_body.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Saz {

	static std::unordered_map<MonoType*, std::function<bool(ecs::Entity)>> s_EntityHasComponentFuncs;

#define SAZ_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Saz.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		SAZ_INFO("{0}, {1}, ", str, parameter);
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		SAZ_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		SAZ_CORE_WARN("Value: {0}", *parameter);
		return glm::dot(*parameter, *parameter);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);
		SAZ_CORE_ASSERT(entity != entt::null);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		SAZ_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void Rigidbody2DComponent_ApplyForce(UUID entityID, glm::vec2* force, glm::vec2* point, bool wake)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		auto& rb2d = world->GetComponent<component::Rigidbody2DComponent>(entity);
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForce(b2Vec2(force->x, force->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyForceToCenter(UUID entityID, glm::vec2* force, bool wake)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		auto& rb2d = world->GetComponent<component::Rigidbody2DComponent>(entity);
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForceToCenter(b2Vec2(force->x, force->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		auto& rb2d = world->GetComponent<component::Rigidbody2DComponent>(entity);
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		auto& rb2d = world->GetComponent<component::Rigidbody2DComponent>(entity);
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	// -- Position -- 
	static void TransformComponent_GetPosition(UUID entityID, glm::vec3* outPosition)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		*outPosition = world->GetComponent<component::TransformComponent>(entity).Position;
	}

	static void TransformComponent_SetPosition(UUID entityID, glm::vec3* position)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		world->GetComponent<component::TransformComponent>(entity).Position = *position;
	}

	// -- Rotation --
	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		*outRotation = world->GetComponent<component::TransformComponent>(entity).Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		world->GetComponent<component::TransformComponent>(entity).Rotation = *rotation;
	}

	// -- Scale --
	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		*outScale = world->GetComponent<component::TransformComponent>(entity).Scale;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetEntity(entityID);

		world->GetComponent<component::TransformComponent>(entity).Scale = *scale;
	}

	// -- Input --
	static bool Input_IsKeyPressed(Input::KeyCode keyCode)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetGameInputEntity(); 
		return world->GetComponent<component::InputComponent>(entity).IsKeyPressed(keyCode);
	}

	static bool Input_IsKeyHeld(Input::KeyCode keyCode)
	{
		ecs::EntityWorld* world = ScriptEngine::GetWorld();
		ecs::Entity entity = world->GetGameInputEntity();

		return world->GetComponent<component::InputComponent>(entity).IsKeyHeld(keyCode);
	}


	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string_view componentString = "Component";
				structName = structName.substr(0, structName.find(componentString));
				std::string managedTypename = fmt::format("Saz.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					SAZ_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](ecs::Entity entity) { return ScriptEngine::GetWorld()->HasComponent<Component>(entity); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		SAZ_ADD_INTERNAL_CALL(NativeLog);

		SAZ_ADD_INTERNAL_CALL(Input_IsKeyPressed);
		SAZ_ADD_INTERNAL_CALL(Input_IsKeyHeld);

		SAZ_ADD_INTERNAL_CALL(Entity_HasComponent);

		SAZ_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		SAZ_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
		SAZ_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		SAZ_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		SAZ_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		SAZ_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		SAZ_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyForce);
		SAZ_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyForceToCenter);
		SAZ_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		SAZ_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
	}
}