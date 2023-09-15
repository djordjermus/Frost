#include "Frost/event_system.hpp"
#include "Frost/log.hpp"
#include "Frost/pimpl.hpp"
#include <iostream>
class base final : public frost::pimpl_crtp<base>
{
public:
	base();
	base(u64 value);
	base(base& move);
	base(base&& move);

};

int main()
{
	base a(5);
	base b(10);
	auto x = b.get_pimpl();
	b.swap_pimpl(a);
}


base::base() : frost::pimpl_crtp<base>() {}
base::base(u64 value) : frost::pimpl_crtp<base>((void*)value) {}
base::base(base& move) : frost::pimpl_crtp<base>(move) {}
base::base(base&& move) : frost::pimpl_crtp<base>(move) {}