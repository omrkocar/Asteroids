#include "GamePCH.h"
#include "SceneEditor.h"

#include "Saz/GameTime.h"
#include <Saz/Screen.h>
#include <Saz/SFML/Window.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>

namespace ecs
{	
	SceneEditor::SceneEditor(Saz::sfml::Window& sfmlWindow)
		: m_SFMLWindow(sfmlWindow)
	{
		
	}

	void SceneEditor::Init()
	{

	}

	void SceneEditor::Update(const Saz::GameTime& gameTime)
	{
		Render();
	}

	void SceneEditor::Render()
	{
		if (!m_IsVisible)
			return;

		/*ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_NoCollapse;
		flags |= ImGuiWindowFlags_NoScrollbar;
		flags |= ImGuiWindowFlags_NoScrollWithMouse;

		if (ImGui::Begin("Scene", &m_IsVisible, flags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			sf::RenderTexture& renderTexture = m_SFMLWindow.m_Texture;

			const sf::Vector2f regionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
			const sf::Vector2f viewSize = renderTexture.getView().getSize();
			Screen::width = regionSize.x;
			Screen::height = regionSize.y;

			ImGui::Image(renderTexture.getTexture(), viewSize);
		}
		ImGui::End();*/
	}
}
