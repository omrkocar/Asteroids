#pragma once

#include <Saz/Core/Application.h>

class Application final : public Saz::Application
{
public:
	Application();
	virtual ~Application() override;

	virtual void Init();
	virtual void Register();
	virtual void Destroy();
	virtual void Update(const Saz::GameTime& gameTime);
};

