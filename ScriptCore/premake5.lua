
-----------------------------------------------------
-- ScriptCore
-----------------------------------------------------
project "ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Editor/Resources/Scripts")
	objdir ("%{wks.location}/Editor/Resources/Scripts/Intermediates")

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Source/ScriptCore/**.cs",  } },
	}

	files
	{
		"Source/ScriptCore/**.cs"
	}