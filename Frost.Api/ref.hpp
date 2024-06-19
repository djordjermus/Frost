#include "include.hpp"
#pragma once
class ref final
{
private:
	frost::api::object* _ptr;
	static inline frost::api::object* inc(frost::api::object* ptr) { if (ptr != nullptr) frost_api_object_acquire_reference(ptr); return ptr; }
	static inline frost::api::object* dec(frost::api::object* ptr) { if (ptr != nullptr) frost_api_object_release_reference(ptr); return ptr; }

public:
	/* CONSTRUCTORS */
	inline ref() :
		_ptr(nullptr) {}
	inline ref(frost::api::object* ptr) :
		_ptr(inc(ptr)) {}
	inline ref(const ref& reference) :
		_ptr(inc(reference._ptr)) {}
	inline ref(const ref&& reference) noexcept :
		_ptr(inc(reference._ptr)) {}
	inline ~ref() { dec(_ptr); }

	/* OPERATORS */
	inline ref& operator=(frost::api::object* ptr)
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

	inline operator frost::api::object* ()
	{
		return _ptr;
	}
	inline operator bool() const
	{
		return _ptr != nullptr;
	}

	inline bool operator == (frost::api::object* ptr) const
	{
		return _ptr == ptr;
	}
	inline bool operator != (frost::api::object* ptr) const
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

	inline frost::api::object* operator->() const
	{
		return _ptr;
	}

	/* METHODS */
	inline frost::api::object* get() const
	{
		return _ptr;
	}

	inline frost::api::object* const* get_array() const
	{
		return &_ptr;
	}
};