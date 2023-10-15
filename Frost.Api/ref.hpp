#include "object.api.hpp"
#pragma once
template<class T>
class ref final
{
private:
	T* _ptr;
	static T* inc(T* ptr) { if (ptr != nullptr) ptr->acquire_reference(); return ptr; }
	static T* dec(T* ptr) { if (ptr != nullptr) ptr->release_reference(); return ptr; }

public:
	/* CONSTRUCTORS */
	ref() :
		_ptr(nullptr) {}
	ref(T* ptr) :
		_ptr(inc(ptr)) {}
	ref(const ref& reference) :
		_ptr(inc(reference._ptr)) {}
	ref(const ref&& reference) noexcept :
		_ptr(inc(reference._ptr)) {}
	~ref() { dec(_ptr); }

	/* OPERATORS */
	ref& operator=(T* ptr)
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
	bool operator == (T* ptr) const { return _ptr == ptr; }
	bool operator != (T* ptr) const { return _ptr != ptr; }
	bool operator == (ref rhs) const { return _ptr == rhs._ptr; }
	bool operator != (ref rhs) const { return _ptr != rhs._ptr; }

	T* operator->() const
	{
		return _ptr;
	}

	/* METHODS */
	T* get() const
	{
		return _ptr;
	}

	T* const* get_array() const
	{
		return &_ptr;
	}
};