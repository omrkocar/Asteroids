
-----------------------------------------------------
-- ScriptCore
-----------------------------------------------------
project "ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Editor/Resources/Scripts")
	objdir ("%{wks.location}/Editor/Resources/Scripts/Intermediates")

	files
	{
		"Source/ScriptCore/**.cs"
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