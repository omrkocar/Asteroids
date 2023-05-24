#include "SazPCH.h"
#include "WindowBase.h"
#include "WindowsWindow.h"

namespace Saz
{
	Saz::WindowBase* WindowBase::Create(const WindowProps& props)
	{
#ifdef SAZ_PLATFORM_WINDOWS
		
		return new WindowsWindow(props);
#else
		SAZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}