#pragma once

#include "Saz/Rendering/SceneCamera.h"
#include "Saz/Rendering/EditorCamera.h"

enum class ViewType
{
	Orthographic,
	Perspective,
	Max = Perspective
};


namespace component
{
	struct EditorCameraComponent
	{
		Saz::EditorCamera Camera;

		EditorCameraComponent() = default;
		EditorCameraComponent(const EditorCameraComponent&) = default;
	};

	struct CameraComponent
	{
		Saz::SceneCamera Camera;
		bool FixedAspectRatio = false;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
