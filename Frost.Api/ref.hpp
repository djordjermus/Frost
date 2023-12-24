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
	inline ref() :
		_ptr(nullptr) {}
	inline ref(frost::api::resource* ptr) :
		_ptr(inc(ptr)) {}
	inline ref(const ref& reference) :
		_ptr(inc(reference._ptr)) {}
	inline ref(const ref&& reference) noexcept :
		_ptr(inc(reference._ptr)) {}
	inline ~ref() { dec(_ptr); }

	/* OPERATORS */
	inline ref& operator=(frost::api::resource* ptr)
	{
		dec(_ptr);
		_ptr = inc(ptr);
		return *this;
	}
	inline ref& operator=(const ref& reference)
	{
		dec(_ptr);
		_ptr = inc(reference._ptr);
		return *this;
	}
	inline ref& operator=(ref&& reference) noexcept
	{
		dec(_ptr);
		_ptr = inc(reference._ptr);
		return *this;
	}

	inline operator frost::api::resource* ()
	{
		return _ptr;
	}
	inline operator bool() const
	{
		return _ptr != nullptr;
	}

	inline bool operator == (frost::api::resource* ptr) const
	{
		return _ptr == ptr;
	}
	inline bool operator != (frost::api::resource* ptr) const
	{
		return _ptr != ptr;
	}

	inline bool operator == (ref rhs) const
	{
		return _ptr == rhs._ptr;
	}
	inline bool operator != (ref rhs) const
	{
		return _ptr != rhs._ptr;
	}

	inline frost::api::resource* operator->() const
	{
		return _ptr;
	}

	/* METHODS */
	inline frost::api::resource* get() const
	{
		return _ptr;
	}

	inline frost::api::resource* const* get_array() const
	{
		return &_ptr;
	}
};