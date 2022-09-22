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
		"SAZ_PROFILE=0"
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

	filter "system:windows"
		systemversion "latest"
		defines{
			"SAZ_PLATFORM_WINDOWS",
		}

	filter "files:%{wks.location}/3rdParty/**.cpp"
		flags { "NoPCH" }

	IncludeDir = {}
	IncludeDir["ImGui"] = "%{wks.location}/3rdParty/imgui"
	IncludeDir["entt"] = "%{wks.location}/3rdParty/entt/include"
	IncludeDir["spdlog"] = "%{wks.location}/3rdParty/spdlog/include"
	IncludeDir["GLFW"] = "%{wks.location}/3rdParty/GLFW/GLFW/include"
	IncludeDir["GLAD"] = "%{wks.location}/3rdParty/GLAD/include"
	IncludeDir["glm"] = "%{wks.location}/3rdParty/glm/glm"
	IncludeDir["stb_image"] = "%{wks.location}/3rdParty/stb_image"
	IncludeDir["yaml_cpp"] = "%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/include"
	IncludeDir["ImGuizmo"] = "%{wks.location}/3rdParty/imguizmo/"
	IncludeDir["Box2D"] = "%{wks.location}/3rdParty/box2d/include"
	IncludeDir["Mono"] = "%{wks.location}/3rdParty/mono/include"

	LibraryDir = {}
	LibraryDir["Mono"] = "%{wks.location}/3rdParty/mono/lib/%{cfg.buildcfg}"

	Library = {}
	Library["Mono"] = "%{LibraryDir.Mono}/libmono-static-sgen.lib"


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
		include "3rdParty/yaml-cpp/premake5.lua"
		include "3rdParty/imguizmo/premake5.lua"
		include "3rdParty/Box2D/premake5.lua"
		group ""
	

project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	location "%{wks.location}/Projects/ZERO_CHECK"
	files { "%{wks.location}/premake5.lua" }

	buildcommands { "cd %{wks.location}/Scripts & call Win-GenProjects.bat" }
	buildoutputs { "unused.txt" }