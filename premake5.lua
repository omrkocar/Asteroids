-----------------------------------------------------
-- Workspace
-----------------------------------------------------
workspace "Saz"
	architecture "x64"
	configurations	{ "Debug", "Release" }
	platforms		{ "x64" }
	startproject	"Editor"
	language "C++"
	cppdialect	"C++17"
	toolset "v143"

	outputdir = "%{cfg.buildcfg}_%{cfg.platform}"

	targetdir ("%{wks.location}/Build/%{prj.name}/" .. outputdir )
	objdir ("%{wks.location}/Intermediate/%{prj.name}/" .. outputdir)
	
	disablewarnings { "26812" }

	filter "Debug"
		defines { "_DEBUG", "SAZ_DEBUG", "SAZ_ENABLE_ASSERTS" }
		optimize "Off"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"
	filter "Release"
		defines { "NDEBUG", "SAZ_RELEASE" }
		optimize "Speed"
		runtime "Release"
		buildoptions "/MD"
		symbols "off"
	filter {}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
		"SAZ_PROFILE=1"
	}

	files 
	{ 
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/Code/%{prj.name}/premake5.*",
	}

	vpaths 
	{ 
		{ ["/"] = { "**premake5.lua" } },
	}

	include "Code/Saz/premake5.lua"
	include "Code/Core/premake5.lua"
	include "Code/Editor/premake5.lua"
	include "Code/Game/premake5.lua"

	group "UnitTest"
		include "Code/Core_ut/premake5.lua"
		include "Code/Saz_ut/premake5.lua"
	group ""

	group "3rdParty"
		include "3rdParty/imgui/premake5.lua"
		include "3rdParty/GLFW/premake5.lua"
		include "3rdParty/GLAD/premake5.lua"
		include "3rdParty/glm/premake5.lua"
		include "3rdParty/stb_image/premake5.lua"
		group ""
	

project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	location "%{wks.location}/Projects/ZERO_CHECK"
	files { "%{wks.location}/premake5.lua" }

	buildcommands { "cd %{wks.location}/Scripts & call Win-GenProjects.bat" }
	buildoutputs { "unused.txt" }