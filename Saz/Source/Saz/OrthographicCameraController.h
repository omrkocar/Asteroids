#pragma once

#include "Saz/Core/GameTime.h"


namespace Saz
{
	/*struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(const GameTime& gameTime);

		void ResizeBounds(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }


	private:
		float m_AspectRatio;
		float m_ZoomLevel = 5.0f;

		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraRotation = 0.0f;
	};*/
}