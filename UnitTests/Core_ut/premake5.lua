project "Core_ut"
	kind "ConsoleApp"
	dependson { "Core" }

	defines { "T_UNIT_TEST" }

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Core_ut/**.h", 
			"Core_ut/**.cpp", 
			"Core_ut/**.inl" } },
	}

	files
	{
		"%{wks.location}/UnitTests/Core_ut/Core_ut/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Core/Source",
		"%{IncludeDir.glm}",
	}
	
	links 
	{ 
		"Core"
	}