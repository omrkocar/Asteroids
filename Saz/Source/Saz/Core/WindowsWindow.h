#pragma once

#include "Saz/Core/WindowBase.h"

#include <Vulkan/vulkan.h>
#include "Core/Vector.h"

struct GLFWwindow;

namespace Saz
{

	class WindowsWindow : public Saz::WindowBase
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate(const Saz::GameTime& gameTime) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		bool ShouldClose() const override;
		void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const override;
		void GatherMouse(Set<Input::MouseCode>& out_Keys, Vector2& out_Delta, Vector2& out_Position) const override;
		void* GetNativeWindow() const override;
		virtual bool HasResized() const override;

		void SetResized(bool resized) { m_Resized = resized; }

		Vector2Int GetSize() const override;

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

	private:
		GLFWwindow* m_Window;

		String m_Name = {};
		Vector2Int m_Size = {};
		bool m_Resized = false;
	};

}