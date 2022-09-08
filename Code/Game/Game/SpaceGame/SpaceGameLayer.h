#pragma once

#include <Saz/Core/Layer.h>
#include "Saz/Events/MouseEvent.h"
#include "Saz/Events/ApplicationEvent.h"
#include "Level.h"

namespace Saz { class OrthographicCamera; }

namespace Game
{
	class SpaceGameLayer : public Saz::Layer
	{
	public:
		SpaceGameLayer();
		virtual ~SpaceGameLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(const Saz::GameTime& gameTime) override;
		void OnImGuiRender() override;
		void OnEvent(Saz::Event& event) override;
		bool OnMouseButtonPressed(Saz::MouseButtonPressedEvent& e);
		bool OnWindowResize(Saz::WindowResizeEvent& e);

	private:
		void CreateCamera(uint32_t width, uint32_t height);
	private:
		Saz::Scope<Saz::OrthographicCamera> m_Camera;
		Level m_Level;
	};
}