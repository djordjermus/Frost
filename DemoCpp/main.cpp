#include "Frost/sync/mutex.hpp"
#include <iostream>
int main()
{
	frost::sync::mutex mx(true);
	bool result;
	result = mx.release();
	result = mx.acquire();
	result = mx.acquire();
}
