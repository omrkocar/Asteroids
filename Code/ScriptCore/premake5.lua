
-----------------------------------------------------
-- ScriptCore
-----------------------------------------------------
project "ScriptCore"
	location	"%{wks.location}/Projects/ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Code/Editor/Resources/Scripts")
	objdir ("%{wks.location}/Code/Editor/Resources/Scripts/Intermediates")

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ScriptCore/**.cs",  } },
	}

	files
	{
		"ScriptCore/**.cs"
	}