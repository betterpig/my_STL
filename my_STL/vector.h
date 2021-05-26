#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

//#include <iterator>
#include "allocator.h"


namespace stl
{
	template<typename T, typename Alloc = stl::allocator<T>>
	class vector
	{
	public:
		typedef T           value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;

	protected:
		stl::allocator<int> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;

		void insert_aux(iterator positon, const_reference x)
		{
			if (end() != end_of_storage)
			{
				data_allocator.construct(finish, x);
				++finish;
			}
			else
			{
				const size_type old_cap = capacity();
				const size_type new_cap = old_cap != 0 ? 2 * old_cap : 1;
				iterator new_start = data_allocator.allocate(new_cap);
				iterator new_finish = new_start;
				for (; start != finish; ++start,++new_finish)
					*new_finish = *start;
				data_allocator.construct(new_finish, x);
				++new_finish;
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_cap;
			}
		}
		void deallocate()
		{
			if (start)
				data_allocator.deallocate(start, end_of_storage - start);
		}
		void fill_initialize(size_type n, const_reference value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
		iterator allocate_and_fill(size_type n, const_reference x)
		{
			iterator result = data_allocator.allocate(n);
			stl::uninitialized_fill_n(result, n, x);
			return result;
		}

	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() { return size_type(end() - begin()); }
		size_type capacity() { return size_type(end_of_storage - begin()); }
		bool empty() { return begin() == end(); }
		reference operator[](size_type n) { return *(begin() + n); }
		
		vector() { start = finish = end_of_storage = nullptr; }
		vector(size_type n, const_reference value) { fill_initialize(n, value); }
		vector(int n, const_reference value) { fill_initialize(n, value); }
		vector(long n, const_reference value) { fill_initialize(n, value); }
		vector(iterator first, iterator last)
		{
			start = data_allocator.allocate(size_type(last-first));
			finish = end_of_storage = stl::uninitialized_fill_copy(first, last, start);
		}

		~vector()
		{
			if (begin() != end())
			{
				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(),capacity());
			}
		}
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		void push_back(const_reference x)
		{
			if (end() != end_of_storage)
			{
				data_allocator.construct(finish, x);
				++finish;
			}
			else
			{
				insert_aux(finish, x);
			}
		}
		void pop_back()
		{
			--finish;
			data_allocator.destroy(finish);
			if (finish == start)
			{
				start = finish = end_of_storage = nullptr;
			}
		}
	};
}





#endif