
-----------------------------------------------------
-- Editor
-----------------------------------------------------
project "Editor"
	kind		"ConsoleApp"
	dependson { "Saz" }
	pchheader "EditorPCH.h"
	pchsource "Source/Editor/EditorPCH.cpp"
	staticruntime "off"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Editor/**.h", 
			"Editor/**.cpp", 
			"Editor/**.inl" } },
	}

	files
	{
		"%{wks.location}/Data/Shaders/*.vert",
		"%{wks.location}/Data/Shaders/*.gLsl",
		"%{wks.location}/Data/Shaders/*.frag",
		"%{wks.location}/Data/**",
	}

	includedirs {
		"%{wks.location}/Code/Editor/",
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/imgui",
		"%{wks.location}/Core/Source",
		"%{wks.location}/Saz/Source",
		"%{wks.location}/Editor/Source",
		"%{wks.location}/3rdParty/GLFW/GLFW/include",
		"%{wks.location}/3rdParty/GLAD/include",
		"%{wks.location}/3rdParty/spdlog/include",
		"%{wks.location}/3rdParty/glm/glm",
		"%{IncludeDir.ImGuizmo}",
	}

	links 
	{
		"Saz",
	}

	filter "system:windows"
		systemversion "latest"