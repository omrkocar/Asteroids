#pragma once

#include "Saz/Core/Application.h"

namespace Game
{
	class Application : public Saz::Application
	{
	public:
		Application();
		virtual ~Application() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Register() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
	};
}
