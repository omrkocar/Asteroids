#pragma once


#include <Saz/Core/Log.h>
#include <Saz/Core/Layer.h>

#include <Saz/Core/Application.h>
#include <Saz/Core/Core.h>
#include <Saz/Core/GameTime.h>
#include <Saz/Core/Entity.h>

#include <Saz/ImGui/ImGuiLayer.h>
#include <Saz/ImGui/ImGuiLog.h>

#include <Saz/Core/KeyCodes.h>
#include <Saz/Core/MouseCodes.h>
#include <Saz/Core/Input.h>

#include "Saz/Rendering/Buffer.h"
#include "Saz/Rendering/OrthographicCamera.h"
#include "Saz/OrthographicCameraController.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Rendering/Shader.h"
#include "Saz/Rendering/Texture.h"
#include "Saz/Rendering/VertexArray.h"
#include "Saz/Rendering/SubTexture2D.h"
#include "Saz/Rendering/FrameBuffer.h"

#include "Saz/Debug/Instrumentor.h"

#include <Core/DynamicArray.h>
#include <Core/Map.h>
#include <Core/Math.h>
#include <Core/FilePath.h>
#include <Core/Random.h>
#include <Core/Matrix.h>
#include <Core/Set.h>
#include <Core/String.h>
#include <Core/Vector.h>
#include <Core/Color.h>
#include <Core/Quaternion.h>
#include <Core/TypeList.h>