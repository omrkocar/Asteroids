#pragma once

namespace ecs { class EntityWorld; }

namespace Saz
{
	class SceneSerializer
	{
	public:
		SceneSerializer(ecs::EntityWorld& world);

		void Serialize(const String& filepath);
		void SerializeRuntime(const String& filepath);

		bool Deserialize(const String& filepath);
		bool DeserializeRuntime(const String& filepath);

		void SerializeLastOpenScene(const String& filepath, const String& lastOpenScenePath);
		String DeserializeLastOpenScene(const String& filepath);

	private:
		ecs::EntityWorld& m_World;
	};
}

