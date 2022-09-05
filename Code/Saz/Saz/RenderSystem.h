#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;

	class WindowBase;
}
namespace ecs 
{
	class SAZ_API RenderSystem final : public System
	{
	public:
		
		RenderSystem(Saz::WindowBase& window);
		~RenderSystem();

		virtual void PostInit() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		Saz::WindowBase& m_Window;
	};
}
