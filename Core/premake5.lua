
project "Core"
	kind "StaticLib"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Core/**.h", 
			"Core/**.cpp", 
			"Core/**.inl" } },
	}

	includedirs 
	{
		"%{wks.location}/Core/Source/",
		"%{IncludeDir.glm}",
	}
