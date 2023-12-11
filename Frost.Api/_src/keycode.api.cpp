#include "../keycode.api.hpp"
#include "../internal/sysmap.impl.hpp"
FROST_API bool _stdcall keycode_to_wcs(
	frost::api::keycode keycode,
	wchar_t* output,
	u64 output_length,
	bool normalize_case)
{
	return frost::impl::sysmap::keycode_to_wcs(keycode, output, output_length, normalize_case);
}
