#pragma once

enum class WindowType { EditorViewport, GameViewport };
namespace component
{

	struct WindowResizedOneFrameComponent
	{
		uint32_t Width;
		uint32_t Height;
		WindowType WindowType;
	};
}