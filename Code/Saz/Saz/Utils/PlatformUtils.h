#pragma once

#include <Core/String.h>

namespace Saz
{
	class FileDialogs
	{
	public:
		// These return empty string if canceled
		static String OpenFile(const char* filter);
		static String SaveFile(const char* filter);
	};
}