project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	
	files 
	{ 
		"%{wks.location}/premake5.lua",
		"premake5.lua",
	}

	buildcommands { "cd %{wks.location}/Scripts & call Win-GenProjects.bat" }
	buildoutputs { "unused.txt" }