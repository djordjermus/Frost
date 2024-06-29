#include "object.hpp"
FROST_API const frost::api::object* _stdcall frost_api_blob_create_from_buffer(void* buffer, u64 byte_size)
{
	frost::impl::blob* result = new frost::impl::blob();
	result->type = frost::api::object_type::data_blob;
	result->data = buffer;
	result->byte_size = byte_size;
	return result;
}

FROST_API const frost::api::object* _stdcall frost_api_blob_copy_from_buffer(void* buffer, u64 byte_size)
{
	frost::impl::blob* result = new frost::impl::blob();
	result->type = frost::api::object_type::data_blob;
	result->data = new u8[byte_size];
	::memcpy(result->data, buffer, byte_size);
	result->byte_size = byte_size;
	return result;
}

FROST_API const void* _stdcall frost_api_blob_get_data(frost::api::object* target)
{
	switch (target->type)
	{
	case frost::api::object_type::data_blob:
		return ((frost::impl::blob*)target)->data;
	default:
		return nullptr;
	}
}

FROST_API const u64 _stdcall frost_api_blob_get_byte_size(frost::api::object* target)
{
	switch (target->type)
	{
	case frost::api::object_type::data_blob:
		return ((frost::impl::blob*)target)->byte_size;
	default:
		return 0;
	}
}