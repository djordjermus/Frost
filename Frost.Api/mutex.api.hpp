#include "synchronizable.api.hpp"
#pragma once
namespace frost::api
{
	class mutex : public synchronizable 
	{
	public:
		static FROST_API mutex* _stdcall create(bool initial_owner);
	};
}