#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"


#include "Saz/GLFW/Window.h"

#include <entt/entt.hpp>
#include "GameTime.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace
{
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};


	constexpr unsigned int indices[] =
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
}

namespace ecs
{
	RenderSystem::RenderSystem(Saz::glfw::Window& window)
		: m_GLFWWindow(window)
		, shader{ "D:/Dev/Saz/Data/Shaders/Basic.vert", "D:/Dev/Saz/Data/Shaders/Basic.frag" }
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	RenderSystem::~RenderSystem()
	{
		
	}

	void RenderSystem::PostInit()
	{
		
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		shader.SetFloat("color", 1.0f);

		glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
