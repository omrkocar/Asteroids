project "Core_ut"
	kind "ConsoleApp"
	dependson { "Core" }
	location "%{wks.location}/Projects/Core_ut"

	defines { "T_UNIT_TEST" }

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Core_ut/**.h", 
			"Core_ut/**.cpp", 
			"Core_ut/**.inl" } },
	}

	includedirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/3rdParty/Vulkan/Include/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/"
	}
	
	links 
	{ 
		"Core.lib" 
	}