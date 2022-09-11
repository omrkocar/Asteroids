project "Game"
	kind "ConsoleApp"
	dependson { "Core", "Saz" }
	pchheader "GamePCH.h"
	pchsource "Game/GamePCH.cpp"
	location "%{wks.location}/Projects/Game"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Game/**.h", 
			"Game/**.cpp", 
			"Game/**.inl" } },
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
		"%{wks.location}/Code/Game/",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{wks.location}/3rdParty/GLFW/GLFW/include",
		"%{wks.location}/3rdParty/GLAD/include",
		"%{wks.location}/3rdParty/imgui",
		"%{wks.location}/3rdParty/glm/glm",
		"%{wks.location}/3rdParty/stb_image",
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/include",
		"%{IncludeDir.ImGuizmo}",
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