#include "../include.hpp"

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


FROST_API void _stdcall frost_api_version_get_api_version(frost::api::version* output)
{
	output->major_version =
		(__DATE__[7] - '0') * 1000+
		(__DATE__[8] - '0') * 100 +
		(__DATE__[9] - '0') * 10 +
		(__DATE__[10] - '0');

	output->minor_version	= 
		(BUILD_MONTH_IS_JAN) ? 1 :
		(BUILD_MONTH_IS_FEB) ? 2 :
		(BUILD_MONTH_IS_MAR) ? 3 :
		(BUILD_MONTH_IS_APR) ? 4 :
		(BUILD_MONTH_IS_MAY) ? 5 :
		(BUILD_MONTH_IS_JUN) ? 6 :
		(BUILD_MONTH_IS_JUL) ? 7 :
		(BUILD_MONTH_IS_AUG) ? 8 :
		(BUILD_MONTH_IS_SEP) ? 9 :
		(BUILD_MONTH_IS_OCT) ? 10 :
		(BUILD_MONTH_IS_NOV) ? 11 :
		(BUILD_MONTH_IS_DEC) ? 12 :
		/* error default */  0
		;
	output->build_number			=
		((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) +
		(__DATE__[5] - '0');
	output->revision = 0;
}