#pragma once

#include <Saz/Systems/System.h>
#include <filesystem>
#include "Saz/Rendering/Texture.h"

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class ContentBrowser final : public System
	{
	public:
		
		ContentBrowser();

		virtual void Init() override;

		virtual void Update(const Saz::GameTime& gameTime) override;
		void ImGuiRender();

		bool IsVisible() { return m_IsActive; }
		void SetVisible(bool active) { m_IsActive = active; }

	private:
		bool m_IsActive = false;
		std::filesystem::path m_CurrentDirectory;

		Saz::Ref<Saz::Texture2D> m_DirectoryIcon;
		Saz::Ref<Saz::Texture2D> m_FileIcon;
	};
}
