#pragma once

#include <Saz/Core/Application.h>

class Application final : public Saz::Application
{
public:
	Application();
	virtual ~Application() override;

	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Register() override;
	virtual void Update(const Saz::GameTime& gameTime) override;
};

