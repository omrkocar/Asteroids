project "RayLib"
	kind "StaticLib"
	language "C++"
	location	"%{wks.location}/Projects/RayLib"
    cppdialect "C++17"

	RayIncludeDir = {}
	RayIncludeDir["RayLib"] = "%{wks.location}/3rdParty/raylib-cpp/vendor/raylib"

	files 
	{
		"%{RayIncludeDir.RayLib}/src/*.h",
		"%{RayIncludeDir.RayLib}/src/*.c",
		"%{wks.location}/3rdParty/raylib-cpp/premake5.lua",
		"%{wks.location}/3rdParty/raylib-cpp/include/**.hpp"
	}

	includedirs 
	{
		"%{RayIncludeDir.RayLib}/src",
		"%{RayIncludeDir.RayLib}/src/external/glfw/include",
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