#include "SazPCH.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"

namespace Saz {

#define SAZ_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Saz.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		SAZ_INFO("{0}, {1}, ", str, parameter);
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		SAZ_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		SAZ_CORE_WARN("Value: {0}", *parameter);
		return glm::dot(*parameter, *parameter);
	}

	void ScriptGlue::RegisterFunctions()
	{
		SAZ_ADD_INTERNAL_CALL(NativeLog);
		SAZ_ADD_INTERNAL_CALL(NativeLog_Vector);
		SAZ_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}