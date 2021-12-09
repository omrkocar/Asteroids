project "Saz_ut"
	kind "ConsoleApp"
	dependson { "Core", "Saz" }
	location "%{wks.location}/Projects/Saz_ut"

	defines { "SAZ_UNIT_TEST" }

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Saz_ut/**.h", 
			"Saz_ut/**.cpp", 
			"Saz_ut/**.inl" } },
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/spdlog/include/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Saz/",
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