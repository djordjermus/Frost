#include "../keycode.api.hpp"
#include "../internal/sysmap.impl.hpp"
FROST_API bool _stdcall keycode_to_wcs(frost::api::keycode keycode, wchar_t* output, u64 output_length)
{
	return frost::impl::sysmap::keycode_to_wcs(keycode, output, output_length);
}
