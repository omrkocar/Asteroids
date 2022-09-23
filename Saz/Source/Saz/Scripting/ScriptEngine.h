#pragma once

#include <filesystem>
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace ecs
{
	class EntityWorld;
}

namespace Saz
{
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, const ecs::Entity& entity, ecs::EntityWorld* world);

		void InvokeInit();
		void InvokeUpdate(float deltaTime);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_InitMethod = nullptr;
		MonoMethod* m_UpdateMethod = nullptr;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(ecs::EntityWorld* world);
		static void OnRuntimeStop(ecs::EntityWorld* world);

		static bool EntityClassExists(const String& fullClassName);
		static void OnCreateEntity(ecs::EntityWorld* world, const ecs::Entity& entity);
		static void OnUpdateEntity(ecs::EntityWorld* world, const ecs::Entity& entity, float deltaTime);

		static MonoImage* GetCoreAssemblyImage();
		static ecs::EntityWorld* GetWorld();

		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
	private:
		static void LoadAssemblyClasses(MonoAssembly* assembly);
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};
}