#pragma once

#include <Saz/EntityWorld.h>
#include <Saz/WindowBase.h>

#include <Vulkan/vulkan.h>

namespace Saz
{
	class ResourceManager;
	class GameTime;

	namespace glfw
	{
		class Window;
	}

	namespace sfml
	{
		class Window;
	}
}

namespace gl
{
	class Window;
}

namespace imgui
{
	class Log;
}

namespace Saz
{
	class Window;

	class SAZ_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run(int argc, char* argv[]);

		inline static Application& Get() { return *s_Instance; }

		Saz::ResourceManager* GetResourceManager() { return m_pResourceManager; }

	protected:
		virtual void Init();
		virtual void PostInit();
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);

		const ecs::EntityWorld& GetWorld();
		


		Saz::sfml::Window* m_SFMLWindow = nullptr;
		Saz::glfw::Window* m_GLFWWindow = nullptr;
		gl::Window* m_GLWindow = nullptr;

		Saz::ResourceManager* m_pResourceManager;

		ecs::EntityWorld m_EntityWorld;

		imgui::Log* m_ImGuiLog = nullptr;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

