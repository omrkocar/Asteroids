#pragma once

#include <Saz/Layer.h>

#include <Saz/Events/KeyEvent.h>
#include <Saz/Events/MouseEvent.h>
#include <Saz/Events/ApplicationEvent.h>

namespace Saz
{
	namespace imgui
	{
		class Log;
	}

	class SAZ_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender();

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}