#pragma once

#include "Saz/Events/Event.h"
#include <Core/String.h>

namespace Saz {
	class GameTime;

	class SAZ_API Layer
	{
	public:
		Layer(const String& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const GameTime& gameTime) {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}