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

namespace vulkan
{
	class Pipeline;
	class Device;
	class SwapChain;
	class Model;
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

	protected:
		virtual void Init();
		virtual void PostInit();
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);
		void DrawFrame();

		const ecs::EntityWorld& GetWorld();
		Saz::ResourceManager* GetResourceManager() { return m_pResourceManager; }


		Saz::sfml::Window* m_SFMLWindow = nullptr;
		Saz::glfw::Window* m_GLFWWindow = nullptr;

		std::unique_ptr<vulkan::Device> m_Device;
		DynamicArray<VkCommandBuffer> m_CommandBuffers;

		Saz::ResourceManager* m_pResourceManager;

		ecs::EntityWorld m_EntityWorld;

		imgui::Log* m_ImGuiLog = nullptr;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

