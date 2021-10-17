#include "SazPCH.h"
#include "FileHelpers.h"

namespace Saz::file
{
	char* LoadCompleteFile(const char* filename, long* length)
	{
		char* filecontents = 0;

		FILE* filehandle;
		errno_t error = fopen_s(&filehandle, filename, "rb");

		if (filehandle)
		{
			fseek(filehandle, 0, SEEK_END);
			long size = ftell(filehandle);
			rewind(filehandle);

			filecontents = new char[size + 1];
			fread(filecontents, size, 1, filehandle);
			filecontents[size] = 0;

			if (length)
				*length = size;

			fclose(filehandle);
		}

		return filecontents;
	}

	void JSONSaveCharArray(WriterType& writer, const char* key, const char* type)
	{
		writer.Key(key);
		writer.String(type);
	}

	void JSONSaveInt(WriterType& writer, const char* key, int value)
	{
		writer.Key(key);
		writer.Int(value);
	}

	void JSONSaveVec2(WriterType& writer, const char* key, vec2 value)
	{
		writer.Key(key);
		writer.StartArray();
		writer.Double(value.x);
		writer.Double(value.y);
		writer.EndArray();
	}

	void JSONSaveVec3(WriterType& writer, const char* key, vec3 value)
	{
		writer.Key(key);
		writer.StartArray();
		writer.Double(value.x);
		writer.Double(value.y);
		writer.Double(value.z);
		writer.EndArray();
	}


	void JSONLoadVec3(rapidjson::Value& object, const char* key, vec3* value)
	{
		SAZ_ASSERT(value, "Value is nullptr");

		if (object.HasMember(key))
		{
			value->x = object[key][0].GetFloat();
			value->y = object[key][1].GetFloat();
			value->z = object[key][2].GetFloat();
		}
	}

	void JSONLoadInt(rapidjson::Value& object, const char* key, int* value)
	{
		SAZ_ASSERT(value, "Value is nullptr");

		if (object.HasMember(key))
		{
			*value = object[key].GetInt();
		}
	}

	void JSONLoadVec2(rapidjson::Value& object, const char* key, vec2* value)
	{
		SAZ_ASSERT(value, "Value is nullptr");

		if (object.HasMember(key))
		{
			value->x = object[key][0].GetFloat();
			value->y = object[key][1].GetFloat();
		}
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