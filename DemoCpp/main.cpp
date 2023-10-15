#include "Frost.Api/object.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
class my_object : public frost::api::object
{
	u64 _value;
protected:
	my_object(u64 value);
	~my_object();
public:
	static my_object* create(u64 value);
};
int main()
{
	ref<my_object> obj = my_object::create(5);
	ref<my_object> obj2 = obj;
	obj = nullptr;
}
my_object::my_object(u64 value) : _value(value) {}
my_object::~my_object() { std::cout << "delete my_object " << _value << '\n'; }
my_object* my_object::create(u64 value) { return new my_object(value); }