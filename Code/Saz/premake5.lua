
-----------------------------------------------------
-- Saz
-----------------------------------------------------

project "Saz"
	kind		"StaticLib"
	dependson { "Core", "Imgui", "GLFW", "Glad" }
	location	"%{wks.location}/Projects/Saz"
	pchheader "SazPCH.h"
	pchsource "Saz/SazPCH.cpp"

	defines 
	{
		"GLFW_INCLUDE_NONE"
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Saz/**.h", 
			"Saz/**.cpp", 
			"Saz/**.inl" } },
	}

	flags
	{
		"MultiProcessorCompile"
	}

	IncludeDir = {}
	IncludeDir["ImGui"] = "%{wks.location}/3rdParty/imgui"
	IncludeDir["entt"] = "%{wks.location}/3rdParty/entt/include"
	IncludeDir["spdlog"] = "%{wks.location}/3rdParty/spdlog/include"
	IncludeDir["GLFW"] = "%{wks.location}/3rdParty/GLFW/include"
	IncludeDir["GLAD"] = "%{wks.location}/3rdParty/GLAD/include"

	includedirs {
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
	}

	libdirs
	{
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/ImGui/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links {
		"Core",
		"ImGui",
		"GLFW",
		"GLAD",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"SAZ_PLATFORM_WINDOWS",
			"SAZ_BUILD_DLL",
		}

	filter "files:%{wks.location}/3rdParty/**.cpp"
		flags { "NoPCH" }

	