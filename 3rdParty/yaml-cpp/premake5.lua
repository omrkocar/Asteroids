project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	files
	{
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/src/**.h",
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/src/**.cpp",
		
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/include/**.h"
	}

	includedirs
	{
		"%{wks.location}/3rdParty/yaml-cpp/yaml-cpp/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
