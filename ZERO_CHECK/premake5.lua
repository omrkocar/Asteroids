project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	
	buildcommands { "cd %{wks.location}/Scripts & call Win-GenProjects.bat" }
	buildoutputs { "unused.txt" }