-----------------------------------------------------
-- Game
-----------------------------------------------------

local SazRootDir = '../..'

workspace "Game"
	startproject "Game"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release",
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "Game"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediate")

	files
	{
		"Source/**.cs",
		"premake5.lua",
		"Win-GenProjects.bat"
	}

	links
	{
		"ScriptCore"
	}

	vpaths 
	{ 
		{ 
			["Source/*"] = 
			{
				"**.cs",  
			} 
		},
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

group "Saz"
	include (SazRootDir .. "/ScriptCore")
group ""