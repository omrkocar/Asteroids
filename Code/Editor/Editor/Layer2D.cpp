#include "EditorPCH.h"
#include "Layer2D.h"
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Layer2D::Layer2D()
	: Layer("Layer2D")
	, m_CameraController(1280.0f / 720.0f, true)
{

}

void Layer2D::OnAttach()
{
	m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Ship.png");
}

void Layer2D::OnUpdate(const Saz::GameTime& gameTime)
{
	SAZ_PROFILE_FUNCTION();

	{
		SAZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(gameTime);
	}

	// Render
	{
		SAZ_PROFILE_SCOPE("Renderer Prep");
		Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Saz::RenderCommand::Clear();
	}

	{
		SAZ_PROFILE_SCOPE("Renderer Draw");
		Saz::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Saz::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.75f, 0.75f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Saz::Renderer2D::DrawQuad({ 0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Saz::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1 }, { 1.0f, 1.0f }, glm::radians(45.0f), m_Texture, 11.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		Saz::Renderer2D::EndScene();
	}
}

void Layer2D::OnDetach()
{

}

void Layer2D::OnEvent(Saz::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Layer2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::End();
}
