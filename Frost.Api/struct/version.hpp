#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	struct version final
	{
		u16 major_version;
		u16 minor_version;
		u16 build_number;
		u16 revision;
	};
}
