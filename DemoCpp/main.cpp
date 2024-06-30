#include "Frost.Api/include.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

void _stdcall fn(frost::api::window_event* target);
ref window;
ref msg;
static void _stdcall log_handler(void* p_data)
{
	frost::api::log_event_data* e = reinterpret_cast<frost::api::log_event_data*>(p_data);
	std::wcout << e->message << L"\n";
}

char buffer[800'000];
int main()
{
	std::ifstream infile("gunshot.wav", std::ios::binary);
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	// Don't overflow the buffer!
	if (length > sizeof(buffer)) {
		length = sizeof(buffer);
	}
	if (!infile.read(buffer, length))
		int i;
	infile.close();
	//frost_api_audio_test();
	// Read file

	char ascii[5] = {};

	u32 id = frost_api_riff_get_chunk_id(buffer);
	u32 file_type = frost_api_riff_get_type(buffer);
	void* list = frost_api_riff_get_subchunk(buffer, frost_api_riff_get_list_id());
	frost_api_riff_id_to_ascii(file_type, ascii);
	bool is_riff = frost_api_riff_get_riff_id() == frost_api_riff_get_chunk_id(buffer);
	auto chunk_count = frost_api_riff_get_subchunk_count(buffer);
	auto riff_fmt = frost_api_riff_get_subchunk(buffer, frost_api_riff_ascii_to_id("fmt "));
	auto riff_data = frost_api_riff_get_subchunk(buffer, frost_api_riff_ascii_to_id("data"));
	auto riff_data_size = frost_api_riff_get_chunk_size(riff_data);

	frost_api_audio_test(frost_api_riff_get_chunk_data(riff_fmt), frost_api_riff_get_chunk_data(riff_data), riff_data_size);

	frost::api::window_description desc = frost::api::window_description();
	desc.procedure = fn;
	window = frost_api_window_create(&desc);
	msg = frost_api_thread_message_create();

	while (frost_api_window_get_state(window) != frost::api::window_state::hidden)
	{
		frost_api_synchronizable_wait(msg);
		frost_api_thread_message_dispatch(msg);
	}
}

void _stdcall fn(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down && e->key_down.key == frost::api::keycode::escape)
		frost_api_window_set_state(e->target, frost::api::window_state::hidden);
	frost_api_logging_log(0x00, L"Hello, World!", 13, nullptr, 0,frost::api::log_level::error);
}