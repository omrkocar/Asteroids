project "RayLib"
	kind "StaticLib"
	language "C++"
	location	"%{wks.location}/Projects/RayLib"
    cppdialect "C++17"

	files 
	{
		"%{wks.location}/3rdParty/raylib/src/*.h", 
		"%{wks.location}/3rdParty/raylib/src/*.c", 
		"%{wks.location}/3rdParty/raylib/premake5.lua"
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/raylib/src",
		"%{wks.location}/3rdParty/raylib/src/external/glfw/include",
	}

	defines{"PLATFORM_DESKTOP"}
	if (_OPTIONS["opengl43"]) then
		defines{"GRAPHICS_API_OPENGL_43"}
	else
		defines{"GRAPHICS_API_OPENGL_33"}
	end

	links 
	{
		"winmm",
		"kernel32",
		"opengl32",
		"kernel32",
		"gdi32"
	}