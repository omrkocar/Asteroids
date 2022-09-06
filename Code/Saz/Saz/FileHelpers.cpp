#include "SazPCH.h"
#include "FileHelpers.h"

namespace Saz::file
{
	const char* LoadCompleteFile(const char* filename)
	{
		char* filecontents = 0;

		FILE* filehandle;
		errno_t error = fopen_s(&filehandle, filename, "rb");

		if (error != 0)
			return 0;

		if (filehandle)
		{
			fseek(filehandle, 0, SEEK_END);
			long size = ftell(filehandle);
			rewind(filehandle);

			filecontents = new char[size + 1];
			fread(filecontents, size, 1, filehandle);
			filecontents[size] = 0;

			fclose(filehandle);
		}

		return filecontents;
	}

	bool LoadFileAsChar(const FilePath& filepath, DynamicArray<char>& out_DynamicArray)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open())
			return false;

		const size_t fileSize = (size_t)file.tellg();
		out_DynamicArray.resize(fileSize);
		file.seekg(0);

		file.read(out_DynamicArray.data(), fileSize);
		file.close();

		return true;
	}

}