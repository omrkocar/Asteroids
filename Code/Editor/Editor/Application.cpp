#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/Inspector.h"
#include "Editor/Layer2D.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/NameComponent.h>
#include <Saz/Systems/LevelSystem.h>

#include <entt/entt.hpp>
#include "Saz/LevelComponent.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.inl"

class ExampleLayer : public Saz::Layer
{
public:
	ExampleLayer()
		: Layer("Placeholder"),
		m_CameraController(1280.0f / 720.0f, true),
		m_SquarePosition(0.0f)
	{
		m_VertexArray = Saz::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Saz::Ref<Saz::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Saz::VertexBuffer::Create(vertices, sizeof(vertices)));
		Saz::BufferLayout layout = {
			{ Saz::ShaderDataType::Float3, "a_Position" },
			{ Saz::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Saz::Ref<Saz::IndexBuffer> indexBuffer;
		indexBuffer.reset(Saz::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Saz::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Saz::Ref<Saz::VertexBuffer> squareVB;
		squareVB.reset(Saz::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Saz::ShaderDataType::Float3, "a_Position" },
			{ Saz::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Saz::Ref<Saz::IndexBuffer> squareIB;
		squareIB.reset(Saz::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		auto textureShader = m_ShaderLibrary.Load("C:/Dev/SazEngine/Data/Shaders/Texture.glsl");
		std::dynamic_pointer_cast<Saz::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Saz::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_FlatColorShader = Saz::Shader::Create("C:/Dev/SazEngine/Data/Shaders/Basic.glsl");

		m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Island.png");
		m_LogoTexture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/ChernoLogo.png");
	}

	void OnUpdate(const Saz::GameTime& gameTime)
	{
		m_CameraController.OnUpdate(gameTime);

		// Render
		Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Saz::RenderCommand::Clear();


		Saz::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 50; y++)
		{
			for (int x = 0; x < 50; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Saz::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Saz::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Saz::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Saz::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(Saz::Event& event)
	{
		m_CameraController.OnEvent(event);
	}

private:
	Saz::ShaderLibrary m_ShaderLibrary;
	Saz::Ref<Saz::Shader> m_Shader;
	Saz::Ref<Saz::VertexArray> m_VertexArray;

	Saz::Ref<Saz::Shader> m_FlatColorShader;
	Saz::Ref<Saz::VertexArray> m_SquareVA;

	Saz::Ref<Saz::Texture2D> m_Texture, m_LogoTexture;

	Saz::OrthographicCameraController m_CameraController;

	float m_SquareMoveSpeed = 2.0f;
	glm::vec3 m_SquarePosition;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


Application::Application()
{
	//PushLayer(new ExampleLayer());
	PushLayer(new Layer2D());
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();

	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();
	m_EntityWorld.RegisterSystem<ecs::Inspector>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
}

void Application::Destroy()
{
	Saz::Application::Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();
}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
