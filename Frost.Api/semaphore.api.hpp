#include "synchronizable.api.hpp"
#pragma once
namespace frost::api
{
	class semaphore : public synchronizable
	{
	public:
		static FROST_API semaphore* _stdcall create(i32 count, i32 max);
	};
}