-----------------------------------------------------
-- Workspace
-----------------------------------------------------
workspace "Asteroids"
	configurations	{ "Debug", "Release" }
	platforms		{ "x64" }
	location		"build"
	characterset	"MBCS"
	startproject	"Asteroids"

	filter "configurations:Debug"
		symbols		"on"

-----------------------------------------------------
-- Asteroids Project
-----------------------------------------------------
project "Asteroids"
	kind		"WindowedApp"
	location	"build/Asteroids"
	debugdir	"Asteroids"
	cppdialect	"C++17"									-- Changing language standard to support std::filesystem

	files {
		"Source/**.cpp",
		"Source/**.h",
		"premake5.lua",
		".gitignore",
		"GenerateProjectFiles.bat",
		"readme.txt",
	}

	includedirs {
		"Source",
	}

	links {
		
	}

	--pchheader "GamePCH.h"
	--pchsource "Game/Source/WinMain.cpp"
