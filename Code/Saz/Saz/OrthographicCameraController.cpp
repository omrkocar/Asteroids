#include "SazPCH.h"
//#include "OrthographicCameraController.h"
//#include "Saz/Core/KeyCodes.h"
//
//namespace Saz
//{
//
//	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
//		: m_AspectRatio(aspectRatio),
//		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), 
//		m_Rotation(rotation)
//	{
//
//	}
//
//	void OrthographicCameraController::OnUpdate(const GameTime& gameTime)
//	{
//		m_Camera.SetPosition(m_CameraPosition);
//
//		m_CameraSpeed = m_ZoomLevel;
//	}
//
//	void OrthographicCameraController::ResizeBounds(float width, float height)
//	{
//		m_AspectRatio = width / height;
//		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
//	}

	/*bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
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
	}*/
//}