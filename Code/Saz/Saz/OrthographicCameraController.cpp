#include "SazPCH.h"
#include "OrthographicCameraController.h"
#include "Saz/Core/Input.h"
#include "Saz/Core/KeyCodes.h"

namespace Saz
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
		: m_AspectRatio(aspectRatio),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), 
		m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(const GameTime& gameTime)
	{
		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraSpeed * gameTime.GetDeltaTime();

		if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraSpeed * gameTime.GetDeltaTime();

		if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraSpeed * gameTime.GetDeltaTime();

		if (Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraSpeed * gameTime.GetDeltaTime();

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation -= m_CameraRotationSpeed * gameTime.GetDeltaTime();
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation += m_CameraRotationSpeed * gameTime.GetDeltaTime();
			
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SAZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SAZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::ResizeBounds(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = Math::Max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		ResizeBounds((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}