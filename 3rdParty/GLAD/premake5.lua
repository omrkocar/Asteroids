project "GLAD"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
    location	"%{wks.location}/Projects/GLAD"

	files
	{
		"include/glad.h",
		"src/glad.c",
		"%{wks.location}/3rdParty/GLAD/premake5.lua"
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/GLAD/include"
	}
