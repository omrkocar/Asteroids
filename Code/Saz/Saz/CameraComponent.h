#pragma once

#include <Core/Matrix.h>
#include <Core/Vector.h>


enum class ViewType
{
	Orthographic,
	Perspective,
	Max = Perspective
};


//namespace component
//{
//	struct CameraComponent
//	{
//		SceneCamera Camera;
//		bool Primary = true; // TODO: think about moving to Scene
//		bool FixedAspectRatio = false;
//
//		CameraComponent() = default;
//		CameraComponent(const CameraComponent&) = default;
//	};
//}