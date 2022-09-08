#include "GamePCH.h"
#include "SpaceGameLayer.h"
#include "Game/Application.h"

#include <Saz/Rendering/Renderer2D.h>
#include "Saz/Rendering/OrthographicCamera.h"
#include "imgui/imgui.h"
#include "Saz/Rendering/RenderCommand.h"

namespace Game
{
	SpaceGameLayer::SpaceGameLayer()
		: Saz::Layer("SpaceGameLayer")
	{
		auto& window = Game::Application::Get().GetWindow();
		CreateCamera(window.GetWidth(), window.GetHeight());
		m_Camera->SetPosition({0.0f, 0.0f, 0.0f });
	}

	void SpaceGameLayer::OnAttach()
	{
		m_Level.Init();
	}

	void SpaceGameLayer::OnDetach()
	{

	}

	void SpaceGameLayer::OnUpdate(const Saz::GameTime& gameTime)
	{
		SAZ_PROFILE_FUNCTION();

		const auto& playerPos = m_Level.GetPlayer().GetPosition();
		m_Level.OnUpdate(gameTime);

		Saz::Renderer2D::ResetStats();

		// Render
		Saz::RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 1.0f });
		Saz::RenderCommand::Clear();

		Saz::Renderer2D::BeginScene(*m_Camera);
		m_Level.OnRender(*m_Camera);
		Saz::Renderer2D::EndScene();
	}

	void SpaceGameLayer::OnImGuiRender()
	{
		m_Level.OnImGuiRender();
	}

	void SpaceGameLayer::OnEvent(Saz::Event& event)
	{
		Saz::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Saz::WindowResizeEvent>(SAZ_BIND_EVENT_FN(SpaceGameLayer::OnWindowResize));

		m_Level.OnEvent(event);
	}

	bool SpaceGameLayer::OnMouseButtonPressed(Saz::MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool SpaceGameLayer::OnWindowResize(Saz::WindowResizeEvent& e)
	{
		CreateCamera(e.GetWidth(), e.GetHeight());
		return false;
	}

	void SpaceGameLayer::CreateCamera(uint32_t width, uint32_t height)
	{
		float aspectRatio = (float)width / (float)height;
		float camWidth = 5.0f;
		float bottom = -camWidth;
		float top = camWidth;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;
		m_Camera = Saz::CreateScope<Saz::OrthographicCamera>(left, right, bottom, top);
	}

}