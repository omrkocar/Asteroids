#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/Inspector.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/ResourceManager.h>
#include <Saz/NameComponent.h>
#include <Saz/LevelSystem.h>
#include <Saz/GameTime.h>

#include <entt/entt.hpp>
#include "Saz/LevelComponent.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include "Saz/Rendering/Shader.h"
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.inl"
#include "Saz/Rendering/Texture.h"

class ExampleLayer : public Saz::Layer
{
public:
	ExampleLayer()
		: Layer("Placeholder"), 
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f), 
		m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Saz::VertexArray::Create());

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

		m_SquareVA.reset(Saz::VertexArray::Create());

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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Saz::Shader::Create(vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_FlatColorShader = Saz::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;
			
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;


			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader = Saz::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc);
		m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Code/Editor/Data/Textures/Island.png");

		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(const Saz::GameTime& gameTime)
	{
		if (Saz::Input::IsKeyPressed(Saz::Key::A))
			m_CameraPosition.x -= m_CameraSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::D))
			m_CameraPosition.x += m_CameraSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::W))
			m_CameraPosition.y += m_CameraSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::S))
			m_CameraPosition.y -= m_CameraSpeed * gameTime.GetDeltaTime();

		if (Saz::Input::IsKeyPressed(Saz::Key::Right))
			m_CameraRotation -= m_CameraRotationSpeed * gameTime.GetDeltaTime();
		if (Saz::Input::IsKeyPressed(Saz::Key::Left))
			m_CameraRotation += m_CameraRotationSpeed * gameTime.GetDeltaTime();

		if (Saz::Input::IsKeyPressed(Saz::Key::J))
			m_SquarePosition.x -= m_SquareMoveSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::L))
			m_SquarePosition.x += m_SquareMoveSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::I))
			m_SquarePosition.y += m_SquareMoveSpeed * gameTime.GetDeltaTime();

		else if (Saz::Input::IsKeyPressed(Saz::Key::K))
			m_SquarePosition.y -= m_SquareMoveSpeed * gameTime.GetDeltaTime();

		Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Saz::RenderCommand::Clear();

		
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Saz::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Saz::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				//Saz::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Saz::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		
		// Triangle
		//Saz::Renderer::Submit(m_Shader, m_VertexArray);
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
		Saz::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Saz::KeyPressedEvent>(SAZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Saz::KeyPressedEvent& event)
	{
		return false;
	}

private:
	Saz::Ref<Saz::Shader> m_Shader;
	Saz::Ref<Saz::VertexArray> m_VertexArray;

	Saz::Ref<Saz::Shader> m_FlatColorShader, m_TextureShader;
	Saz::Ref<Saz::VertexArray> m_SquareVA;

	Saz::Ref<Saz::Texture2D> m_Texture;

	Saz::OrtographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	float m_SquareMoveSpeed = 2.0f;
	glm::vec3 m_SquarePosition;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


Application::Application()
{
	PushLayer(new ExampleLayer());
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

void Application::DrawMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Level"))
	{
		if (ImGui::MenuItem("Clear Level"))
		{
			m_EntityWorld.DestroyAllEntities();
			m_EntityWorld.GetSystem<ecs::WorldOutliner>().m_IsObjectInspectorOn = false;
		}

		if (ImGui::MenuItem("Load Default Level"))
		{
			ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
			levelSystem.LoadFromFile("DefaultScene.scene");
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Debug"))
	{
		auto& registry = m_EntityWorld.m_Registry;
		if (ImGui::MenuItem("Create Entity"))
		{
			static int index = 0;
			ecs::Entity entity = m_EntityWorld.CreateEntity();

			component::RenderComponent& renderComp = m_EntityWorld.AddComponent<component::RenderComponent>(entity);
			m_EntityWorld.AddComponent<component::LevelComponent>(entity);
			auto& nameComp = m_EntityWorld.AddComponent<component::NameComponent>(entity);
			nameComp.m_Name = "Object(" + std::to_string(index) +  ")";
			index++;
			component::TransformComponent& transformComp = m_EntityWorld.AddComponent<component::TransformComponent>(entity);
			transformComp.m_Position = vec2(Random::Range(0.0f, 1000.0f), Random::Range(0.0f, 500.0f));
			//IMGUI_LOG_INFO("A new entity is created at %f, %f", transformComp.m_Position.x, transformComp.m_Position.y);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
