
-----------------------------------------------------
-- Editor
-----------------------------------------------------
project "Editor"
	kind		"ConsoleApp"
	dependson { "Saz" }
	location	"%{wks.location}/Projects/Editor"
	pchheader "EditorPCH.h"
	pchsource "Editor/EditorPCH.cpp"
	staticruntime "on"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Editor/**.h", 
			"Editor/**.cpp", 
			"Editor/**.inl" } },
	}

	files
	{
		"%{wks.location}/Data/Shaders/*.vert",
		"%{wks.location}/Data/Shaders/*.gLsl",
		"%{wks.location}/Data/Shaders/*.frag",
	}

	includedirs {
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/imgui/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
		"%{wks.location}/Code/Editor/",
		"%{wks.location}/3rdParty/GLFW/include",
		"%{wks.location}/3rdParty/GLAD/include",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{wks.location}/3rdParty/glm",
		"%{wks.location}/3rdParty/stb_image",
		"%{wks.location}/3rdParty/yaml-cpp/include"
	}

	libdirs{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Build/Saz/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{
		"Saz",
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"SAZ_PLATFORM_WINDOWS"
		}