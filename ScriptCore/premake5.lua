
-----------------------------------------------------
-- ScriptCore
-----------------------------------------------------
project "ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../Editor/Resources/Scripts")
	objdir ("../Editor/Resources/Scripts/Intermediates")

	files
	{
		"Source/ScriptCore/**.cs",
		"premake5.lua"
	}

	vpaths 
	{ 
		{ 
			["Source/*"] = 
			{
				"**.cs",  
			} 
		},
	}