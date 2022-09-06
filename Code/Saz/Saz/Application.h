#pragma once

#include <Saz/EntityWorld.h>
#include <Saz/LayerStack.h>
#include <Saz/WindowBase.h>
#include "Saz/Events/ApplicationEvent.h"


namespace Saz
{
	class ResourceManager;
	class GameTime;
}

namespace imgui
{
	class Log;
}

namespace Saz::imgui
{
	class Log;
}

namespace Saz
{

	class SAZ_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		inline static Application& Get() { return *s_Instance; }

		Saz::ResourceManager* GetResourceManager() { return m_pResourceManager; }

		inline WindowBase& GetWindow() { return *m_Window; }

	protected:
		virtual void Init();
		virtual void PostInit();
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);

		void OnEvent(Event& e);

		const ecs::EntityWorld& GetWorld();

		bool OnWindowClose(WindowCloseEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
		std::unique_ptr<WindowBase> m_Window = nullptr;


		Saz::ResourceManager* m_pResourceManager;

		ecs::EntityWorld m_EntityWorld;

		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;

		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

