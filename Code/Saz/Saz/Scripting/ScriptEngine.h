#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}


namespace Saz
{
	class ScriptEngine
	{
	public:
		
		static void Init();
		static void Shutdown();

	private:
		static void InitMono();
		static void ShutdownMono();
	};
}