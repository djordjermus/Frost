#include "Frost/event_system.hpp"
#include "Frost/log.hpp"
#include <iostream>
class TextClass
{
public:
	const char* text = "Hello World!";
	static void print(TextClass* text_class)
	{
		std::cout << text_class->text << '\n';
	}
};
void PrintLogEvent(frost::log::event_data* e)
{
	std::wcout << e->message_template << L'\n';
	std::wcout << e->message << L'\n';

	std::wcout << L"PARAMS:\n";
	for (auto& param : e->parameters)
		std::wcout << L'\t' << param << L'\n';

	std::wcout << e->timestamp << L'\n';
	std::wcout << e->thread_id << L'\n';
	std::wcout << (int)e->level << L'\n';
}
int main()
{
	frost::event_system::subscribe(1, PrintLogEvent);
	frost::log::critical(1, L"{0}, {1}, {2}, {3}, {", {L"", L"abc", L"123"});
}