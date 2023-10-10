#pragma once
namespace frost
{
	template<class T> class impl_t;
	template<class T> using pimpl_t = impl_t<T>*;

	template<class T>
	class pimpl_crtp
	{
	public:
		pimpl_crtp() : _ptr(nullptr) {}
		pimpl_crtp(void* ptr) : _ptr(reinterpret_cast<pimpl_t<T>>(ptr)) {}
		pimpl_crtp(pimpl_crtp& move) : _ptr(move._ptr)
		{
			move._ptr = nullptr;
		}
		pimpl_crtp(pimpl_crtp&& move) : _ptr(move._ptr)
		{
			move._ptr = nullptr;
		}

		T* operator->()
		{
			return reinterpret_cast<T*>(_ptr);
		}
		const T* operator->() const
		{
			return reinterpret_cast<T*>(_ptr);
		}

		pimpl_t<T> get_pimpl() const
		{
			return _ptr;
		}

		[[nodiscard]] pimpl_t<T> swap_pimpl(pimpl_t<T> ptr)
		{
			pimpl_t<T> temp = _ptr;
			_ptr = ptr;
			return temp;
		}

		void swap_pimpl(pimpl_crtp& p_impl)
		{
			pimpl_t<T> temp = _ptr;
			_ptr = p_impl._ptr;
			p_impl._ptr = temp;
		}

	private:
		pimpl_t<T> _ptr;
	};
}