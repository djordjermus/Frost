#include "../clock.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>


#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
