#include "Frost/event_system.hpp"
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
int main()
{
	frost::event_system::subscribe(1, TextClass::print);
	frost::event_system::subscribe(3, TextClass::print);
	TextClass x = TextClass();
	x.text = ":)";
	frost::event_system::emit(1, &x);
}