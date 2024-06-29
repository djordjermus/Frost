#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	class log_parameter final
	{
	public:
		const wchar_t* name;
		const wchar_t* value;
		u64 name_length;
		u64 value_length;
	};
}
