#pragma once
namespace frost::api
{
	struct window_event;
	using window_procedure_sig = void(_stdcall*)(window_event* target);
}