
-----------------------------------------------------
-- Engine
-----------------------------------------------------
project "Engine"
	kind		"ConsoleApp"
	dependson { "Saz" }
	location	"%{wks.location}/Projects/Engine"
	pchheader "EnginePCH.h"
	pchsource "Engine/EnginePCH.cpp"
	staticruntime "on"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Engine/**.h", 
			"Engine/**.cpp", 
			"Engine/**.inl" } },
	}

	files
	{
		"%{wks.location}/Data/**",
	}

	includedirs {
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/spdlog/include/",
		"%{wks.location}/3rdParty/imgui/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
		"%{wks.location}/Code/Engine/",
		"%{wks.location}/3rdParty/GLFW/include",
		"%{wks.location}/3rdParty/GLAD/include",
		"%{wks.location}/3rdParty/glm",
	}

	libdirs{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Build/Saz/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{
		"Saz",
		"ImGui",
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"SAZ_PLATFORM_WINDOWS"
		}