#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;

	class Window;
}
namespace ecs 
{
	class SAZ_API RenderSystem final : public System
	{
	public:
		
		RenderSystem(Saz::Window& window);
		~RenderSystem();

		virtual void PostInit() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		Saz::Window& m_Window;
	};
}
