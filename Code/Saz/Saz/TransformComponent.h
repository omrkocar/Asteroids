#pragma once

#include <Core/Vector.h>

namespace component
{
	struct TransformComponent
	{
		vec3 m_Position = vec3::Zero();
		vec3 m_Rotation = vec3::Zero();
		vec3 m_Scale = vec3::One();

		void AddInspector();
	};

	inline void TransformComponent::AddInspector()
	{
		ImGui::DragFloat3("Position", &m_Position.x, 0.5f);
		ImGui::DragFloat3("Rotation", &m_Rotation.x, 0.5f);
		ImGui::DragFloat3("Scale", &m_Scale.x, 0.5f);
	}

}
