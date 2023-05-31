
-----------------------------------------------------
-- Editor
-----------------------------------------------------
project "Editor"
	kind		"ConsoleApp"
	dependson { "Saz" }
	pchheader "EditorPCH.h"
	pchsource "Source/Editor/EditorPCH.cpp"
	staticruntime "off"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Editor/**.h", 
			"Editor/**.cpp", 
			"Editor/**.inl" } },
	}

	includedirs {
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/imgui",
		"%{wks.location}/Core/Source",
		"%{wks.location}/Saz/Source",
		"%{wks.location}/Editor/Source",
		"%{wks.location}/3rdParty/Vulkan/Include/",
		"%{wks.location}/3rdParty/GLFW/GLFW/include",
		"%{wks.location}/3rdParty/GLAD/include",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{wks.location}/3rdParty/glm/glm",
		"%{wks.location}/3rdParty/stb_image",
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/include",
		"%{IncludeDir.ImGuizmo}",
	}

	libdirs
	{
		"%{wks.location}/3rdParty/Vulkan/Library/",
	}

	links 
	{
		"Saz",
		"vulkan-1"
	}

	filter "system:windows"
		systemversion "latest"