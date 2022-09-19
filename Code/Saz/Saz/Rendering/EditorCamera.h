#pragma once

#include "Camera.h"
#include "Saz/Core/GameTime.h"

#include <glm/glm.hpp>
#include <utility>

namespace Saz {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void Setup(float fov, float aspectRatio, float nearClip, float farClip, float yaw = 0.0f, float pitch = 0.0f, float distance = 10.0f);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		void SetViewportSize(float width, float height);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		float SetPitch(float pitch) { m_Pitch = pitch; }
		float SetYaw(float yaw) { m_Yaw = yaw; }

		void SetFOV(float fov) { m_FOV = fov; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		void SetNearClip(float nearClip) { m_NearClip = nearClip; }
		void SetFarClip(float farClip) { m_FarClip = farClip; }

		float GetFOV() { return m_FOV; }
		float GetAspectRatio() { return m_AspectRatio; }
		float GetNearClip() { return m_NearClip; }
		float GetFarClip() { return m_FarClip; }
		const glm::vec3& GetFocalPoint() { return m_FocalPoint; }
		void SetFocalPoint(const glm::vec3& focalPoint) { m_FocalPoint = focalPoint; }

		bool OnMouseScroll(float YOffset);
	public:
		void UpdateProjection();
		void UpdateView();

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}