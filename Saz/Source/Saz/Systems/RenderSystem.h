#pragma once

#include <Saz/Systems/System.h>
#include "Saz/Vulkan/Renderer.h"
#include "Saz/Vulkan/Device.h"

namespace Saz
{
	class GameTime;
}
namespace ecs 
{
	class RenderSystem final : public System
	{
	public:
		RenderSystem(vulkan::Device& device, vulkan::Renderer& renderer);
		~RenderSystem();

		virtual void Destroy() override;

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		vulkan::Renderer& m_Renderer;
		vulkan::Device& m_Device;
	};
}
