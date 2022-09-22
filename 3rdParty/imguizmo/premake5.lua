project "ImGuizmo"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"

	files
	{
		"imguizmo/*.h",
		"imguizmo/*.cpp",
		"premake5.lua"
	}

	includedirs 
	{ 
		"imguizmo",
		"../imgui/imgui",
	}
