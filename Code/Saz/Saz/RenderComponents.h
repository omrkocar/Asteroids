#pragma once

#include <Saz/Vulkan/Model.h>
#include <memory>
#include <Core/Vector.h>

namespace component
{
	struct RenderComponent
	{
		//sf::Sprite* m_Sprite = nullptr;
		std::shared_ptr<vulkan::Model> model;
		vec3 color;
		//StringView m_Type;
	};
}


