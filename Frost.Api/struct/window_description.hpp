#include "../primitives.hpp"
#include "../enum/window_state.hpp"
#include "../function/window_procedure.hpp"
#include "point.hpp"
#include "size.hpp"
#pragma once
namespace frost::api
{
	struct object;
	struct window_description final
	{
	public:
		point2d<i32> position = { 200, 200 };
		size2d<i32> size = { 1280, 720 };

		window_procedure_sig procedure = nullptr;
		void* data = nullptr;

		window_state state = window_state::normal;

		struct
		{
			frost::api::object* root = nullptr;

			size2d<u32> buffer_size = { 1280, 720 };
			u32 buffer_count = 2;
		} graphics;
	};
}
