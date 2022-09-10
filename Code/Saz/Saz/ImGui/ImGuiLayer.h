#pragma once

#include <Saz/Core/Layer.h>


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

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		void SetupImGuiStyle();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}