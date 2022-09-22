project "Saz_ut"
	kind "ConsoleApp"
	dependson { "Core", "Saz" }

	defines { "SAZ_UNIT_TEST" }

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Saz_ut/**.h", 
			"Saz_ut/**.cpp", 
			"Saz_ut/**.inl" } },
	}

	files
	{
		"%{wks.location}/UnitTests/Saz_ut/Saz_ut/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/spdlog/include/",
		"%{wks.location}/Core/Source",
		"%{wks.location}/Saz/Source",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
	}

	libdirs
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Build/Saz/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{
		"Saz",
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"SAZ_PLATFORM_WINDOWS"
		}