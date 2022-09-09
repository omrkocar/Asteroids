#include "SazPCH.h"
#include "WindowBase.h"

#ifdef SAZ_PLATFORM_WINDOWS
#include "Saz/Platform/Windows/WindowsWindow.h"
#endif


namespace Saz
{
	Scope<Saz::WindowBase> WindowBase::Create(const WindowProps& props /*= WindowProps()*/)
	{
#ifdef SAZ_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		SAZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}