#pragma once

#include <Core/Color.h>
#include "Saz/Vulkan/Model.h"

namespace Saz
{
	class FrameBuffer;
}

namespace component
{
	struct SpriteComponent
	{
		Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
		vulkan::Model* model;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
	};

	struct CircleRendererComponent
	{
		Color Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct FrameBufferComponent
	{
		Saz::Ref<Saz::FrameBuffer> FrameBuffer;
	};
}