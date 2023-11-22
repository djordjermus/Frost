#include "resource.impl.hpp"
#include <typeinfo>
#pragma once
namespace frost::impl
{
	class debug
	{
	public:
		static void log_resource_creation(const frost::api::resource* target);
		static void log_resource_destruction(const frost::api::resource* target, const std::type_info& type);
	};
}
