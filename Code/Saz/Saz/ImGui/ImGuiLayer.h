#pragma once

#include <Saz/Core/Layer.h>

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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}