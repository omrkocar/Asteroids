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
		int placeholder = 0;
	};
}
