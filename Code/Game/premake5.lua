
-----------------------------------------------------
-- Game
-----------------------------------------------------
project "Game"
	kind		"ConsoleApp"
	dependson { "Saz" }
	location	"%{wks.location}/Projects/Game"
	pchheader "GamePCH.h"
	pchsource "Game/GamePCH.cpp"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Game/**.h", 
			"Game/**.cpp", 
			"Game/**.inl" } },
	}

	files
	{
		"%{wks.location}/Data/**",
	}

	includedirs {
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/spdlog/include/",
		"%{wks.location}/3rdParty/glad/include/",
		"%{wks.location}/3rdParty/glad/src/",
		"%{wks.location}/3rdParty/imgui/",
		"%{wks.location}/3rdParty/glad/include",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
		"%{wks.location}/3rdParty/glm/",
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