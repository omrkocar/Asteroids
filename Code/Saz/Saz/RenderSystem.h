#pragma once

#include <Saz/System.h>

#include "Shader.h"

namespace Saz
{
	class GameTime;

	namespace glfw
	{
		class Window;
	}
}
namespace ecs 
{
	class SAZ_API RenderSystem final : public System
	{
	public:
		
		RenderSystem( Saz::glfw::Window& window);
		~RenderSystem();

		virtual void PostInit() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		Saz::glfw::Window& m_GLFWWindow;
		unsigned int VBO, VAO, EBO, shaderProgram;
		Shader shader;
	};
}
