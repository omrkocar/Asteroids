#pragma once

#include "Saz/Core/Layer.h"
#include "glm/ext/vector_float3.hpp"
#include "Saz/OrthographicCameraController.h"
#include "Saz/Rendering/Shader.h"
#include "Saz/Rendering/VertexArray.h"
#include "Saz/Rendering/Texture.h"

namespace Saz { class FrameBuffer; }

namespace Saz { class SubTexture2D; }

class EditorLayer : public Saz::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(const Saz::GameTime& gameTime) override;
	void OnImGuiRender() override;
private:
	Saz::Ref<Saz::Texture2D> m_Texture;
	Saz::Ref<Saz::FrameBuffer> m_FrameBuffer;

	glm::vec2 m_SceneSize = { 0.0f, 0.0f };

	bool m_ViewportFocused = false;
	bool m_ViewPortHovered = false;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};