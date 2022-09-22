project "Game"
	kind "ConsoleApp"
	dependson { "Core", "Saz" }
	pchheader "GamePCH.h"
	pchsource "Source/Game/GamePCH.cpp"

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
		"%{wks.location}/Core/Source",
		"%{wks.location}/Saz/Source",
		"%{wks.location}/Game/Source",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
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