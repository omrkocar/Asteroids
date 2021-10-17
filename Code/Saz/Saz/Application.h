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
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);
		void DrawFrame();

		const ecs::EntityWorld& GetWorld();
		Saz::ResourceManager* GetResourceManager() { return m_pResourceManager; }


	protected:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void LoadModels();
		void RecreateSwapChain();
		void RecordCommandBuffer(int imageIndex);

		Saz::sfml::Window* m_SFMLWindow = nullptr;
		Saz::glfw::Window* m_GLFWWindow = nullptr;

		std::unique_ptr<vulkan::Pipeline> m_Pipeline;
		std::unique_ptr<vulkan::Device> m_Device;
		std::unique_ptr<vulkan::SwapChain> m_SwapChain;
		std::unique_ptr<vulkan::Model> m_Model;
		VkPipelineLayout m_PipelineLayout;
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

