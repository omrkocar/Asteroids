#pragma once

#include "Saz/Core/UUID.h"

namespace component
{
	struct NameComponent
	{
		String Name = "Entity";
	};

	struct IDComponent
	{
		Saz::UUID ID;
	};
}