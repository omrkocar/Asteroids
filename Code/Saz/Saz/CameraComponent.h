#pragma once

#include <Core/Matrix.h>
#include <Core/Vector.h>


enum class ViewType
{
	Orthographic,
	Perspective,
	Max = Perspective
};


namespace component
{
	struct CameraComponent
	{
		Matrix m_ProjectionMatrix = Matrix::Identity;
		Matrix m_ViewMatrix = Matrix::Identity;
		ViewType m_ViewType = ViewType::Perspective;
		vec3 m_MovementAxes = vec3::Zero();
		vec2 m_RotationAxes = vec2::Zero();
	};
}
