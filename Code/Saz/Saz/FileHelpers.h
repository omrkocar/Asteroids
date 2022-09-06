#pragma once

#include <Core/Vector.h>

namespace Saz::file
{
	const char* LoadCompleteFile(const char* filename);
	bool LoadFileAsChar(const FilePath& filepath, DynamicArray<char>& out_DynamicArray);
}