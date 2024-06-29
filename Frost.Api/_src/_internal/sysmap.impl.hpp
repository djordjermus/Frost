#include "../../include.hpp"
#pragma once
#if defined (TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
#include <utility>
namespace frost::impl
{
	class sysmap final
	{
	private:
		static sysmap _instance;

		std::pair<api::keycode, u8> _keycode_map[256];
		api::keycode _button_event_keycode_map[10];

		sysmap();
		~sysmap() = default;

	public:

		static u8 compose_syskey(u16 make_code, u16 flags);

		static u8 keycode_to_syskey(api::keycode internal_keycode);
		static api::keycode syskey_to_keycode(u8 system_keycode);

		static api::keycode system_button_event_to_internal_keycode(u16 event_value, bool* out_down);

		static u64 keycode_to_wcs(api::keycode internal_keycode, wchar_t* output, u64 output_length, bool normalize_case);
		static u64 syskey_to_wcs(u8 system_keycode, wchar_t* output, u64 output_length, bool normalize_case);

	private:
		static void decompose_syskey(u8 syskey, u16* out_make_code, u16* out_flags);
		static u16 compose_scancode(u16 make_code, u16 flags);
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
