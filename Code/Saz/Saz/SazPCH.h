#pragma once

#ifndef NOMINMAX
# define NOMINMAX
#endif

#ifdef SAZ_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <windowsx.h>
#endif

#include <assert.h>
#include <filesystem>
#include <fstream>  
#include <iostream>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>


#include <map>
#include <queue>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

#include <Core/DynamicArray.h>
#include <Core/Map.h>
#include <Core/Input.h>
#include <Core/Math.h>
#include <Core/FilePath.h>
#include <Core/Random.h>
#include <Core/Matrix.h>
#include <Core/Set.h>
#include <Core/String.h>
#include <Core/Vector.h>
#include <Core/Color.h>
#include <Core/Quaternion.h>
#include <Core/TypeList.h>

#include "Saz/Core.h"
#include "Saz/Entity.h"
#include "Saz/TypeId.h"
#include "Saz/Log.h"
#include "Saz/ImGui/ImGuiLog.h"

#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#include <raylib-cpp.hpp>
#include <rlImGui/rlImGui.h>
#include <imgui/imgui.h>