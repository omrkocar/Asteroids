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

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}