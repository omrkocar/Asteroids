project "glm"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
    location	"%{wks.location}/Projects/glm"

	defines
	{
		"GLM_FORCE_CTOR_INIT"
	}

	files
	{
		"**.hpp",
		"**.inl",
		"%{wks.location}/3rdParty/glm/premake5.lua"
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/glm/glm"
	}
