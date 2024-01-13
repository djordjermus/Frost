#include <math.h>
#include "../color.api.hpp"

#define DEG60 (1.0f / 6.0f)
#pragma warning(push)
#pragma warning (disable: 26495)

FROST_API void _stdcall frost_api_color_rgba8_to_rgba32(const u32 input, f32* output)
{
	output[0] = ((input >> 0x00) & 0xFF) / 255.0f;
	output[1] = ((input >> 0x08) & 0xFF) / 255.0f;
	output[2] = ((input >> 0x10) & 0xFF) / 255.0f;
	output[3] = ((input >> 0x18) & 0xFF) / 255.0f;
}
FROST_API void _stdcall frost_api_color_rgba32_to_rgba8(const f32* input, u32* output)
{
	*output =
		((static_cast<u32>(input[0] * 255.0f) << 0x00) & 0xFF) |
		((static_cast<u32>(input[1] * 255.0f) << 0x08) & 0xFF) |
		((static_cast<u32>(input[2] * 255.0f) << 0x10) & 0xFF) |
		((static_cast<u32>(input[3] * 255.0f) << 0x18) & 0xFF);
	}

FROST_API void _stdcall frost_api_color_rgba32_to_hsva(const f32* input, f32* output)
{
	float cmax =
		(input[0] > input[1] ?
			(input[0] > input[2] ? input[0] : input[2]) :
			(input[1] > input[2] ? input[1] : input[2]));

	float cmin =
		(input[0] < input[1] ?
			(input[0] < input[2] ? input[0] : input[2]) :
			(input[1] < input[2] ? input[1] : input[2]));

	float delta = cmax - cmin;

	if (delta == 0)
		output[0] = 0;
	else if (delta == input[0])
		output[0] = DEG60 * fmodf((input[1] - input[2]) / delta, 6);
	else if (delta == input[1])
		output[0] = DEG60 * ((input[2] - input[0]) / delta + 2);
	else if (delta == input[2])
		output[0] = DEG60 * ((input[0] - input[1]) / delta + 2);

	output[1] = cmax == 0.0f ?
		0.0f :
		delta / cmax;

	output[2] = cmax;

	output[3] = input[3];
}
FROST_API void _stdcall frost_api_color_rgba32_to_hsla(const f32* input, f32* output)
{
	float cmax =
		(input[0] > input[1] ?
			(input[0] > input[2] ? input[0] : input[2]) :
			(input[1] > input[2] ? input[1] : input[2]));

	float cmin =
		(input[0] < input[1] ?
			(input[0] < input[2] ? input[0] : input[2]) :
			(input[1] < input[2] ? input[1] : input[2]));

	float delta = cmax - cmin;

	if (delta == 0)
		output[0] = 0;
	else if (delta == input[0])
		output[0] = DEG60 * fmodf((input[1] - input[2]) / delta, 6);
	else if (delta == input[1])
		output[0] = DEG60 * ((input[2] - input[0]) / delta + 2);
	else if (delta == input[2])
		output[0] = DEG60 * ((input[0] - input[1]) / delta + 2);

	output[2] = (cmax + cmin) / 2.0f;

	output[1] = (delta == 0.0f ?
		0.0f :
		delta / (1.0f - fabsf(2.0f * output[2] - 1.0f)));

	output[3] = input[3];
}
FROST_API void _stdcall frost_api_color_rgba32_to_cmyk(const f32* input, f32* output)
{
	float max =
		(input[0] > input[1] ?
			(input[0] > input[2] ? input[0] : input[2]) :
			(input[1] > input[2] ? input[1] : input[2]));

	if (max != 0.0f)
	{
		output[0] = (max - input[0]) / max;
		output[1] = (max - input[1]) / max;
		output[2] = (max - input[2]) / max;
		output[3] = 1.0f - max;
	}
	else
	{
		output[0] = 0.0f;
		output[1] = 0.0f;
		output[2] = 0.0f;
		output[3] = 1.0f;
	}
}

FROST_API void _stdcall frost_api_color_hsva_to_rgba32(const f32* input, f32* output)
{
	float c = input[2] * input[1];
	float x = c * (1.0f - fabsf(fmodf(input[0] / DEG60, 2.0f) - 1.0f));
	float m = input[2] - c;

	if (input[0] >= 0 * DEG60 && input[0] < 1 * DEG60)
	{
		output[0] = c + m; output[1] = x + m; output[2] = 0.0f + m;
	}
	else if (input[0] >= 1 * DEG60 && input[0] < 2 * DEG60)
	{
		output[0] = x + m; output[1] = c + m; output[2] = 0.0f + m;
	}
	else if (input[0] >= 2 * DEG60 && input[0] < 3 * DEG60)
	{
		output[0] = 0 + m; output[1] = c + m; output[2] = x + m;
	}
	else if (input[0] >= 3 * DEG60 && input[0] < 4 * DEG60)
	{
		output[0] = 0 + m; output[1] = x + m; output[2] = c + m;
	}
	else if (input[0] >= 4 * DEG60 && input[0] < 5 * DEG60)
	{
		output[0] = x + m; output[1] = 0.0f + m; output[2] = c + m;
	}
	else if (input[0] >= 5 * DEG60 && input[0] < 6 * DEG60)
	{
		output[0] = c + m; output[1] = 0.0f + m; output[2] = x + m;
	}
}
FROST_API void _stdcall frost_api_color_hsla_to_rgba32(const f32* input, f32* output)
{
	float c = input[1] * (1.0f - fabsf(2 * input[2] - 1.0f));
	float x = c * (1.0f - fabsf(fmodf(input[0] / DEG60, 2.0f) - 1.0f));
	float m = input[2] - (c / 2.0f);

	if (input[0] >= 0 * DEG60 && input[0] < 1 * DEG60)
	{
		output[0] = c + m; output[1] = x + m; output[2] = 0.0f + m;
	}
	else if (input[0] >= 1 * DEG60 && input[0] < 2 * DEG60)
	{
		output[0] = x + m; output[1] = c + m; output[2] = 0.0f + m;
	}
	else if (input[0] >= 2 * DEG60 && input[0] < 3 * DEG60)
	{
		output[0] = 0 + m; output[1] = c + m; output[2] = x + m;
	}
	else if (input[0] >= 3 * DEG60 && input[0] < 4 * DEG60)
	{
		output[0] = 0 + m; output[1] = x + m; output[2] = c + m;
	}
	else if (input[0] >= 4 * DEG60 && input[0] < 5 * DEG60)
	{
		output[0] = x + m; output[1] = 0.0f + m; output[2] = c + m;
	}
	else if (input[0] >= 5 * DEG60 && input[0] < 6 * DEG60)
	{
		output[0] = c + m; output[1] = 0.0f + m; output[2] = x + m;
	}
}
FROST_API void _stdcall frost_api_color_cmyk_to_rgba32(const f32* input, f32* output)
{
	float one_k = 1.0f - output[3];
	output[0] = (1.0f - input[0]) * one_k;
	output[1] = (1.0f - input[1]) * one_k;
	output[2] = (1.0f - input[2]) * one_k;
	output[3] = 1.0f;
}
#pragma warning(pop)
