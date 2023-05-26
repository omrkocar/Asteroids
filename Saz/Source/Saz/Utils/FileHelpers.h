#pragma once

#include <Core/String.h>
#include <Core/DynamicArray.h>
#include <Core/FilePath.h>

namespace Saz::file
{
	String LoadCompleteFile(const char* filename);
	bool LoadFileAsChar(const FilePath& filepath, DynamicArray<char>& out_DynamicArray);
}