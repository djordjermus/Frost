#include "resource.api.hpp"
#pragma once
class ref final
{
private:
	frost::api::resource* _ptr;
	static inline frost::api::resource* inc(frost::api::resource* ptr) { if (ptr != nullptr) resource_acquire_reference(ptr); return ptr; }
	static inline frost::api::resource* dec(frost::api::resource* ptr) { if (ptr != nullptr) resource_release_reference(ptr); return ptr; }

public:
	/* CONSTRUCTORS */
	ref() :
		_ptr(nullptr) {}
	ref(frost::api::resource* ptr) :
		_ptr(inc(ptr)) {}
	ref(const ref& reference) :
		_ptr(inc(reference._ptr)) {}
	ref(const ref&& reference) noexcept :
		_ptr(inc(reference._ptr)) {}
	~ref() { dec(_ptr); }

	/* OPERATORS */
	ref& operator=(frost::api::resource* ptr)
	{
		dec(_ptr);
		_ptr = inc(ptr);
		return *this;
	}
	ref& operator=(const ref& reference)
	{
		dec(_ptr);
		_ptr = inc(reference._ptr);
		return *this;
	}
	ref& operator=(ref&& reference) noexcept
	{
		dec(_ptr);
		_ptr = inc(reference._ptr);
		return *this;
	}

	operator bool() const { return _ptr != nullptr; }
	bool operator == (frost::api::resource* ptr) const { return _ptr == ptr; }
	bool operator != (frost::api::resource* ptr) const { return _ptr != ptr; }
	bool operator == (ref rhs) const { return _ptr == rhs._ptr; }
	bool operator != (ref rhs) const { return _ptr != rhs._ptr; }

	frost::api::resource* operator->() const
	{
		return _ptr;
	}

	/* METHODS */
	frost::api::resource* get() const
	{
		return _ptr;
	}

	frost::api::resource* const* get_array() const
	{
		return &_ptr;
	}
};