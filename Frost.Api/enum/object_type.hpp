#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class object_type : u64
	{
		basic_object			= 0x0000000000'000000,

		data_blob				= 0x0000000000'000001,
		data_blob_inlined		= 0x0000000000'000002,

		sync_mutex				= 0x0000000000'000003,
		sync_semaphore			= 0x0000000000'000004,
		sync_event				= 0x0000000000'000005,

		thread_reference		= 0x0000000000'000006,
		thread					= 0x0000000000'000007,
		thread_message			= 0x0000000000'000008,

		window					= 0x0000000000'000009,

		graphics_root			= 0x0000000000'00000A,

		graphics_command		= 0x0000000000'00000B,

		graphics_input_layout	= 0x0000000000'00000C,
		graphics_input			= 0x0000000000'00000D,

		graphics_texture		= 0x0000000000'00000E,
		graphics_render_target	= 0x0000000000'00000F,
		graphics_depth_stencil	= 0x0000000000'000010,

		graphics_shader_builder	= 0x0000000000'000011,
		graphics_shader			= 0x0000000000'000012,
		graphics_material		= 0x0000000000'000013,
	};
}
