#include "SazPCH.h"
#include "Saz/SceneSerializer.h"

#include "Saz/Components/CameraComponent.h"
#include "Saz/Components/InputComponent.h"
#include "Saz/Components/NameComponent.h"
#include "Saz/Components/PhysicsComponents.h"
#include "Saz/Components/RenderComponents.h"
#include "Saz/Components/SceneComponent.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Core/Application.h"
#include "Saz/Core/EntityWorld.h"
#include "Saz/Rendering/Texture.h"

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Saz
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, ecs::EntityWorld& world, ecs::Entity entity)
	{
		if (!world.HasComponent<component::SceneEntityComponent>(entity))
			return;
		if (world.HasComponent<component::EditorCameraComponent>(entity))
			return;

		SAZ_CORE_ASSERT(world.HasComponent<component::IDComponent>(entity), "");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << world.GetUUID(entity);

		if (world.HasComponent<component::NameComponent>(entity))
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap; // NameComponent

			auto& name = world.GetComponent<component::NameComponent>(entity).Name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap; // NameComponent
		}

		if (world.HasComponent<component::TransformComponent>(entity))
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = world.GetComponent<component::TransformComponent>(entity);
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (world.HasComponent<component::CameraComponent>(entity))
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = world.GetComponent<component::CameraComponent>(entity);
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::Key << "AspectRatio" << YAML::Value << camera.GetAspectRatio();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (world.HasComponent<component::SpriteComponent>(entity))
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			auto& spriteComponent = world.GetComponent<component::SpriteComponent>(entity);
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.Color;
			if (spriteComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteComponent.Texture->GetPath();
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteComponent.TilingFactor;

			out << YAML::EndMap; // SpriteComponent
		}

		if (world.HasComponent<component::CircleRendererComponent>(entity))
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& circleRendererComponent = world.GetComponent<component::CircleRendererComponent>(entity);
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap; // CircleRendererComponent
		}

		if (world.HasComponent<component::Rigidbody2DComponent>(entity))
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent

			auto& rigidbody2D = world.GetComponent<component::Rigidbody2DComponent>(entity);
			out << YAML::Key << "BodyType" << YAML::Value << (int)rigidbody2D.Type;
			out << YAML::Key << "GravityScale" << YAML::Value << rigidbody2D.GravityScale;
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidbody2D.FixedRotation;

			out << YAML::EndMap; // Rigidbody2DComponent
		}

		if (world.HasComponent<component::BoxCollider2DComponent>(entity))
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			auto& boxCollider2D = world.GetComponent<component::BoxCollider2DComponent>(entity);
			out << YAML::Key << "Size" << YAML::Value << boxCollider2D.Size;
			out << YAML::Key << "Offset" << YAML::Value << boxCollider2D.Offset;
			out << YAML::Key << "Density" << YAML::Value << boxCollider2D.Density;
			out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << boxCollider2D.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2D.RestitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

		if (world.HasComponent<component::CircleCollider2DComponent>(entity))
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& boxCollider2D = world.GetComponent<component::CircleCollider2DComponent>(entity);
			out << YAML::Key << "Radius" << YAML::Value << boxCollider2D.Radius;
			out << YAML::Key << "Offset" << YAML::Value << boxCollider2D.Offset;
			out << YAML::Key << "Density" << YAML::Value << boxCollider2D.Density;
			out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << boxCollider2D.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2D.RestitutionThreshold;

			out << YAML::EndMap; // CircleCollider2DComponent
		}

		out << YAML::EndMap; // Entity
	}

	SceneSerializer::SceneSerializer(ecs::EntityWorld& world)
		: m_World(world)
	{
		
	}

	void SceneSerializer::Serialize(const String& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << filepath;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		std::map<UUID, entt::entity> sortedEntityMap;
		auto idComponentView = m_World.m_Registry.view<component::IDComponent>();
		for (auto entity : idComponentView)
			sortedEntityMap[idComponentView.get<component::IDComponent>(entity).ID] = entity;

		// Serialize sorted entities
		for (auto [id, entity] : sortedEntityMap)
			SerializeEntity(out, m_World, entity);

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const String& filepath)
	{
		// Not implemented
		SAZ_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const String& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				ecs::Entity deserializedEntity = m_World.CreateBaseEntity(uuid, "");

				std::string name;
				auto nameComponent = entity["NameComponent"];
				if (nameComponent)
				{
					name = nameComponent["Name"].as<std::string>();
					auto& nameComp = m_World.GetComponent<component::NameComponent>(deserializedEntity);
					nameComp.Name = name;
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = m_World.GetComponent<component::TransformComponent>(deserializedEntity);
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = m_World.AddComponent<component::CameraComponent>(deserializedEntity);

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
					cc.Camera.SetAspectRatio(cameraProps["AspectRatio"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& src = m_World.AddComponent<component::SpriteComponent>(deserializedEntity);
					src.Color = spriteComponent["Color"].as<glm::vec4>();
					if (spriteComponent["TexturePath"])
						src.Texture = Texture2D::Create(spriteComponent["TexturePath"].as<std::string>());

					if (spriteComponent["TilingFactor"])
						src.TilingFactor = spriteComponent["TilingFactor"].as<float>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = m_World.AddComponent<component::CircleRendererComponent>(deserializedEntity);
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidbody2D = entity["Rigidbody2DComponent"];
				if (rigidbody2D)
				{
					auto& rb = m_World.AddComponent<component::Rigidbody2DComponent>(deserializedEntity);
					rb.Type = (Physics::BodyType)rigidbody2D["BodyType"].as<int>();
					rb.GravityScale = rigidbody2D["GravityScale"].as<float>();
					rb.FixedRotation = rigidbody2D["FixedRotation"].as<bool>();
				}

				auto boxCollider2D = entity["BoxCollider2DComponent"];
				if (boxCollider2D)
				{
					auto& bc = m_World.AddComponent<component::BoxCollider2DComponent>(deserializedEntity);
					bc.Size = boxCollider2D["Size"].as<glm::vec2>();
					bc.Offset = boxCollider2D["Offset"].as<glm::vec2>();
					bc.Density = boxCollider2D["Density"].as<float>();
					bc.Friction = boxCollider2D["Friction"].as<float>();
					bc.Restitution = boxCollider2D["Restitution"].as<float>();
					bc.RestitutionThreshold = boxCollider2D["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2D = entity["CircleCollider2DComponent"];
				if (circleCollider2D)
				{
					auto& cc = m_World.AddComponent<component::CircleCollider2DComponent>(deserializedEntity);
					cc.Radius = circleCollider2D["Radius"].as<float>();
					cc.Offset = circleCollider2D["Offset"].as<glm::vec2>();
					cc.Density = circleCollider2D["Density"].as<float>();
					cc.Friction = circleCollider2D["Friction"].as<float>();
					cc.Restitution = circleCollider2D["Restitution"].as<float>();
					cc.RestitutionThreshold = circleCollider2D["RestitutionThreshold"].as<float>();
				}
			}
		}

		SAZ_CORE_TRACE("Deserialized {0}", sceneName);
		m_World.SortEntities();

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const String& filepath)
	{
		// Not implemented
		SAZ_CORE_ASSERT(false, "Not implemented");
		return false;
	}

	void SceneSerializer::SerializeLastOpenScene(const String& filepath, const String& lastOpenScenePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "LastOpenScene" << YAML::Value << lastOpenScenePath;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	String SceneSerializer::DeserializeLastOpenScene(const String& filepath)
	{
		// TODO: improve this later
		std::ifstream stream(filepath);
		std::stringstream strStream;
		if (!strStream.good())
			return "";

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["LastOpenScene"])
			return "";

		std::string sceneName = data["LastOpenScene"].as<std::string>();
		Deserialize(sceneName);
		return sceneName;
	}

}