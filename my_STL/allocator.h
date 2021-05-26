#ifndef MYSTL_ALLOCATOR_H
#define MYSTL_ALLOCATOR_H

#include <utility>



namespace stl
{
	template<typename Forward_iter,typename Size_type,typename Value_type>
	Forward_iter uninitialized_fill_n(Forward_iter start, Size_type n, Value_type x)
	{
		while (n > 0)
		{
			*start = x;
			n--;
			++start;
		}
		return start;
	}

	template<typename Input_iter ,typename Forward_iter>
	Forward_iter uninitialized_fill_copy(Input_iter first, Input_iter last, Forward_iter d_first)
	{
		for (; first != last; ++d_first, (void) ++first)
			::new (static_cast<void*>(d_first)) typename iterator_traits<ForwardIt>::value_type(*first);
		return d_first;
	}

	template <typename T>
	class allocator
	{
	public:
		typedef T           value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;

	public:
		allocator();
		~allocator();
		pointer allocate(size_type n);
		void deallocate(pointer p, size_type n);
		void construct(pointer p, const_reference x);
		void destroy(pointer p);
		void destroy(iterator start, iterator finish);

	private:

	};

	template <typename T>
	allocator<T>::allocator()
	{

	}

	template <typename T>
	allocator<T>::~allocator()
	{
		
	}

	template <typename T>
	typename allocator<T>::pointer allocator<T>::allocate(size_type n)
	{
		if (n == 0)
			return nullptr;
		return static_cast<pointer>(::operator new(n * sizeof(value_type)));
	}

	template <typename T>
	void allocator<T>::deallocate(pointer start, size_type n)
	{
		if (start == nullptr)
			return;
		
		::operator delete(start);
	}

	template <typename T>
	void allocator<T>::construct(pointer p, const_reference x)
	{
		::new ((void*)p) T(x);
	}

	template <typename T>
	void allocator<T>::destroy(pointer p)
	{
		if (p != nullptr)
			p->~T();
	}

	template <typename T>
	void allocator<T>::destroy(iterator start, iterator finish)
	{
		for (; start!=finish; ++start)
			destroy(start);
	}
}


#endif