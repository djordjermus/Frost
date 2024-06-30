#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class object_type : u64
	{
		basic_object,

		data_blob,
		data_blob_inlined,

		sync_mutex,
		sync_semaphore,
		sync_event,

		thread_reference,
		thread,
		thread_message,

		window
	};
}
