#pragma once

#include "Saz/Core/WindowBase.h"
#include <Vulkan/vulkan.h>

struct GLFWwindow;

namespace Saz
{

	class WindowsWindow : public Saz::WindowBase
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate(const Saz::GameTime& gameTime) override;
		void Destroy() override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		const String& GetTitle() const override;
		void SetTitle(const String& title) override;
		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;
		bool ShouldClose() const override;
		void Shutdown() override;
		void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const override;
		void GatherMouse(Set<Input::MouseCode>& out_Keys, vec2& out_Delta, vec2& out_Position) const override;
		void* GetNativeWindow() const override;

	private:
		void Initialize();

	private:
		GLFWwindow* m_Window;
		VkInstance m_VkInstance;
	};

}