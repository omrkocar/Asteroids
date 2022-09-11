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
#include <sstream>

#include <map>
#include <queue>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <utility>

#include <Core/DynamicArray.h>
#include <Core/Map.h>
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

#include "Saz/Core/Core.h"
#include "Saz/Core/Entity.h"
#include "Saz/Core/Log.h"
#include "Saz/Screen.h"
#include "Saz/Core/KeyCodes.h"
#include "Saz/Core/MouseCodes.h"
#include "Saz/Core/GameTime.h"
#include "Saz/Debug/Instrumentor.h"
#include "Saz/Utils/FileHelpers.h"