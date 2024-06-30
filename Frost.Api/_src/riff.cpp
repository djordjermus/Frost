#include "../include.hpp"
#include "string.h"

static u32 riff_id = frost_api_riff_ascii_to_id("RIFF");
static u32 list_id = frost_api_riff_ascii_to_id("LIST");
FROST_API u32 _stdcall frost_api_riff_get_riff_id()
{
	return riff_id;
}

FROST_API u32 _stdcall frost_api_riff_get_list_id()
{
	return list_id;
}

FROST_API u32 _stdcall frost_api_riff_ascii_to_id(const char* id)
{
	try
	{
		u32 result =
			(id[0] << 0) |
			(id[1] << 8) |
			(id[2] << 16) |
			(id[3] << 24);
		return result;
	}
	catch (...)
	{
		return 0;
	}
}

FROST_API bool _stdcall frost_api_riff_id_to_ascii(u32 id, char* out_ascii)
{
	try
	{
		out_ascii[0] = (id >> 0);
		out_ascii[1] = (id >> 8);
		out_ascii[2] = (id >> 16);
		out_ascii[3] = (id >> 24);
		out_ascii[4] = '\0';
		return true;
	}
	catch (...)
	{
		return false;		
	}
}

FROST_API u32 _stdcall frost_api_riff_get_type(const void* chunk)
{
	auto id = frost_api_riff_get_chunk_id(chunk);
	if (id != riff_id && id != list_id)
		return 0;

	const u8* file_name = reinterpret_cast<const u8*>(chunk) + 8;

	u32 result =
		(file_name[0] << 0) |
		(file_name[1] << 8) |
		(file_name[2] << 16) |
		(file_name[3] << 24);
	return result;
}

FROST_API u32 _stdcall frost_api_riff_get_chunk_id(const void* chunk)
{
	try
	{
		u32 result =
			(((const u8*)chunk)[0] << 0) |
			(((const u8*)chunk)[1] << 8) |
			(((const u8*)chunk)[2] << 16) |
			(((const u8*)chunk)[3] << 24);
		return result;
	}
	catch (...)
	{
		return 0;
	}
}

FROST_API u32 _stdcall frost_api_riff_get_chunk_size(const void* chunk)
{
	try
	{
		const u8* ptr = reinterpret_cast<const u8*>(chunk) + 4;
			u32 result =
			(ptr[0] << 0) |
			(ptr[1] << 8) |
			(ptr[2] << 16) |
			(ptr[3] << 24);
		return result;
	}
	catch (...)
	{
		return 0;
	}
}

FROST_API void* _stdcall frost_api_riff_get_chunk_data(void* chunk)
{
	auto id = frost_api_riff_get_chunk_id(chunk);
	if (id != riff_id && id != list_id)
		return reinterpret_cast<u32*>(chunk) + 2;
	else
		return reinterpret_cast<u32*>(chunk) + 3;
}

FROST_API u64 _stdcall frost_api_riff_get_subchunk_count(void* chunk)
{
	auto id = frost_api_riff_get_chunk_id(chunk);
	if (id != riff_id && id != list_id)
		return 0;

	u32 size = frost_api_riff_get_chunk_size(chunk);
	void* end = reinterpret_cast<u8*>(chunk) + 8 + size;
	auto elem = frost_api_riff_get_chunk_data(chunk);
	u64 count = 0;
	while (elem < end)
	{
		id = frost_api_riff_get_chunk_id(elem);
		if (id == 0)
			break;

		size = frost_api_riff_get_chunk_size(elem);
		auto parity = (size & 1);
		elem = reinterpret_cast<u8*>(elem) + 8 + size + (size & 1);
		count++;
	}
	return count;
}

FROST_API void* _stdcall frost_api_riff_get_subchunk_indexed(void* chunk, u64 index)
{
	auto id = frost_api_riff_get_chunk_id(chunk);
	if (id != riff_id && id != list_id)
		return 0;

	u32 size = frost_api_riff_get_chunk_size(chunk);
	void* end = reinterpret_cast<u8*>(chunk) + 8 + size;
	auto elem = frost_api_riff_get_chunk_data(chunk);
	u64 count = 0;
	while (elem < end)
	{
		id = frost_api_riff_get_chunk_id(elem);
		if (id == 0)
			break;

		size = frost_api_riff_get_chunk_size(elem);
		auto parity = (size & 1);
		if (count == index)
			return elem;

		elem = reinterpret_cast<u8*>(elem) + 8 + size + (size & 1);
		count++;
	}
	return nullptr;
}

FROST_API void* _stdcall frost_api_riff_get_subchunk(void* chunk, u32 chunk_id)
{
	auto id = frost_api_riff_get_chunk_id(chunk);
	if (id != riff_id && id != list_id)
		return 0;

	u32 size = frost_api_riff_get_chunk_size(chunk);
	void* end = reinterpret_cast<u8*>(chunk) + 8 + size;
	auto elem = frost_api_riff_get_chunk_data(chunk);
	while (elem < end)
	{
		id = frost_api_riff_get_chunk_id(elem);
		if (id == chunk_id)
			return elem;
		else if (id == 0)
			break;

		size = frost_api_riff_get_chunk_size(elem);
		auto parity = (size & 1);
		elem = reinterpret_cast<u8*>(elem) + 8 + size + (size & 1);
	}
	return nullptr;
}