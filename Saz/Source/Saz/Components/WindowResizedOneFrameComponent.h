#pragma once

enum class WindowType { EditorViewport, GameViewport };
namespace component
{

	struct WindowResizedOneFrameComponent
	{
		float Width;
		float Height;
		WindowType WindowType;
	};
}