#include "SazPCH.h"
#include "WindowBase.h"

namespace Saz
{
	Scope<Saz::WindowBase> WindowBase::Create(const WindowProps& props)
	{
#ifdef SAZ_PLATFORM_WINDOWS
		
		return nullptr;
#else
		SAZ_CORE_ASSERT(false, "Unknown platform!");
#endif
	}

}