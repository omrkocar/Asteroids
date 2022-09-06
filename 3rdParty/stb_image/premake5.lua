project "stb_image"
	kind "StaticLib"
    location	"%{wks.location}/Projects/stb_image"
	files
	{
		"*.cpp",
		"*.h",
		"%{wks.location}/3rdParty/stb_image/premake5.lua"
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/stb_image"
	}
