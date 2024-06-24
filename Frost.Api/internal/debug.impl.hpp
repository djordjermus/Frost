#include "../include.hpp"
#include <typeinfo>
#pragma once
namespace frost::impl
{
	class debug
	{
	public:
		static void log_resource_creation(const frost::api::object* target);
		static void log_resource_destruction(const frost::api::object* target);
	};
}
