#include "object.hpp"
FROST_API frost::api::object* _stdcall frost_api_blob_from_owned_buffer(void* buffer, u64 byte_size)
{
	frost::impl::blob* result = new frost::impl::blob();
	result->type = frost::api::object_type::data_blob;
	result->data = buffer;
	result->byte_size = byte_size;
	return result;
}

FROST_API frost::api::object* _stdcall frost_api_blob_copy_from_buffer(void* buffer, u64 byte_size)
{
	u64 result_size = (sizeof(frost::impl::blob_inlined) + byte_size + sizeof(u64) - 1) / sizeof(u64);
	u64* result_buffer = new u64[result_size];
	frost::impl::blob_inlined* result = reinterpret_cast<frost::impl::blob_inlined*>(result_buffer);
	void* data = result + 1;

	result->type = frost::api::object_type::data_blob_inlined;
	::memcpy(data, buffer, byte_size);
	result->byte_size = byte_size;

	return result;
}

FROST_API void* _stdcall frost_api_blob_get_data(frost::api::object* target)
{
	switch (target->type)
	{
	case frost::api::object_type::data_blob:
		return ((frost::impl::blob*)target)->data;

	case frost::api::object_type::data_blob_inlined:
		return ((frost::impl::blob_inlined*)target) + 1;

	default:
		return nullptr;
	}
}

FROST_API u64 _stdcall frost_api_blob_get_byte_size(frost::api::object* target)
{
	switch (target->type)
	{
	case frost::api::object_type::data_blob:
		return ((frost::impl::blob*)target)->byte_size;

	case frost::api::object_type::data_blob_inlined:
		return ((frost::impl::blob_inlined*)target)->byte_size;

	default:
		return 0;
	}
}