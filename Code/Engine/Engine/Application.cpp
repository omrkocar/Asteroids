#include "EnginePCH.h"

#include "Application.h"

#include "Engine/WorldOutliner.h"
#include "Engine/Inspector.h"

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

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Saz::Layer
{
public:
	ExampleLayer()
		: Layer("Placeholder"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Saz::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Saz::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Saz::VertexBuffer::Create(vertices, sizeof(vertices)));
		Saz::BufferLayout layout = {
			{ Saz::ShaderDataType::Float3, "a_Position" },
			{ Saz::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Saz::IndexBuffer> indexBuffer;
		indexBuffer.reset(Saz::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Saz::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Saz::VertexBuffer> squareVB;
		squareVB.reset(Saz::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Saz::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Saz::IndexBuffer> squareIB;
		squareIB.reset(Saz::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

		m_Shader.reset(new Saz::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Saz::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(const Saz::GameTime& ts)
	{
		if (Saz::Input::IsKeyPressed(Saz::Key::A))
			m_CameraPosition.x -= m_CameraSpeed;

		else if (Saz::Input::IsKeyPressed(Saz::Key::D))
			m_CameraPosition.x += m_CameraSpeed;

		else if (Saz::Input::IsKeyPressed(Saz::Key::W))
			m_CameraPosition.y += m_CameraSpeed;

		else if (Saz::Input::IsKeyPressed(Saz::Key::S))
			m_CameraPosition.y -= m_CameraSpeed;

		if (Saz::Input::IsKeyPressed(Saz::Key::Right))
			m_CameraRotation -= m_CameraRotationSpeed;
		if (Saz::Input::IsKeyPressed(Saz::Key::Left))
			m_CameraRotation += m_CameraRotationSpeed;

		Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Saz::RenderCommand::Clear();

		
		//ImGui::DragFloat("CameraPosition", m_CameraPosition, 0.1f, 0.0f, 5.0f);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Saz::Renderer::BeginScene(m_Camera);
		Saz::Renderer::Submit(m_BlueShader, m_SquareVA);
		Saz::Renderer::Submit(m_Shader, m_VertexArray);
		Saz::Renderer::EndScene();
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
	std::shared_ptr<Saz::Shader> m_Shader;
	std::shared_ptr<Saz::VertexArray> m_VertexArray;

	std::shared_ptr<Saz::Shader> m_BlueShader;
	std::shared_ptr<Saz::VertexArray> m_SquareVA;

	Saz::OrtographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.05f;
	float m_CameraRotationSpeed = 1.0f;
	float m_CameraRotation = 0.0f;
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
