#pragma once

#include <Saz/Systems/System.h>


namespace Saz
{
	class GameTime;	
}

namespace ecs 
{
	class ScriptSystem final : public System
	{
	public:
		ScriptSystem();
		~ScriptSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
	private:
		void OnSceneStateChanged(Entity entity);
	private:
		Entity m_Entity;
		bool m_IsActive = false;
	};
}
