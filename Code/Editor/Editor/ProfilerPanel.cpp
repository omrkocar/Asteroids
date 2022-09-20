#include "EditorPCH.h"
#include "ProfilerPanel.h"
#include "Saz/Rendering/Renderer2D.h"
#include "imgui/imgui.h"

namespace ecs
{
	ProfilerPanel::ProfilerPanel()
	{

	}

	void ProfilerPanel::Init()
	{
		
	}

	void ProfilerPanel::Update(const Saz::GameTime& gameTime)
	{
		if (!m_IsActive)
			return;

		ImGuiRender();
	}

	void ProfilerPanel::SetVisible(bool active)
	{
		ImGui::SetWindowFocus("Profiler");
		m_IsActive = active;
	}

	void ProfilerPanel::ImGuiRender()
	{
		ImGui::Begin("Profiler", &m_IsActive);
		auto stats = Saz::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats: ");
		ImGui::Text("DrawCalls: %d ", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
}
