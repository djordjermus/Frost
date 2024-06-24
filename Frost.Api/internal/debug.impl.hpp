#include "../include.hpp"
#include <typeinfo>
#pragma once
namespace frost::impl
{
	class debug
	{
	public:
		static void log_object_creation(const frost::api::object* target);
		static void log_object_destruction(const frost::api::object* target);
		static void log_object_invalid_type(const frost::api::object* target);
	};
}
