#include "EditorPCH.h"
#include "ContentBrowser.h"

#include "Saz/Core/GameTime.h"

#include <imgui/imgui.h>
#include "Saz/Rendering/Texture.h"
#include "imgui/imgui_internal.h"

namespace ecs
{	
	//#todo: Introduce a directory manager
	extern const std::filesystem::path g_DataPath = "../../Data";

	ContentBrowser::ContentBrowser()
	{
		m_DirectoryIcon = Saz::Texture2D::Create("../../Data/Textures/Folder.png");
		m_FileIcon = Saz::Texture2D::Create("../../Data/Textures/File.png");
	}

	void ContentBrowser::Init()
	{
		m_CurrentDirectory = g_DataPath;
	}

	void ContentBrowser::Update(const Saz::GameTime& gameTime)
	{

	}

	void ContentBrowser::ImGuiRender()
	{
		ImGui::Begin("Content Browser", nullptr);

		if (m_CurrentDirectory != std::filesystem::path(g_DataPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(directoryEntry.path(), g_DataPath);
			String filenameString = relativePath.filename().string();
			filenameString = filenameString.substr(filenameString.find_last_of("/\\") + 1);

			ImGui::PushID(filenameString.c_str());
			Saz::Ref<Saz::Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_DataPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512, "%.0f");
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}
