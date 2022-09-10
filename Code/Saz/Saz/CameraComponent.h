#pragma once

#include "Saz/Rendering/SceneCamera.h"


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
		Saz::SceneCamera Camera;
		bool FixedAspectRatio = false;

		// Perhaps separate SceneCamera from other cameras
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}