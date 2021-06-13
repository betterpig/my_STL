#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

//#include <iterator>
#include "allocator.h"
#include <initializer_list>

namespace stl
{
	template<typename Input_iter, typename Backward_iter>
	Backward_iter backward_copy(Input_iter first, Input_iter last, Backward_iter dlast)
	{
		while (first != last)
			*(--dlast) = *(--last);
		return dlast;
	}



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

	public:
		//**********  容器构造
		vector() { start = finish = end_of_storage = nullptr; }//默认构造函数

		//构造函数
		vector(size_type n, const_reference value) { fill_initialize(n, value); }
		vector(int n, const_reference value) { fill_initialize(n, value); }
		vector(long n, const_reference value) { fill_initialize(n, value); }
		template <typename Input_iter>
		vector(Input_iter first, Input_iter last)
		{
			range_init(first, last);
		}

		vector(std::initializer_list<T> init)
		{
			range_init(init.begin(), init.end());
		}

		//拷贝构造函数
		vector(const vector& rhs)
		{
			range_init(rhs.begin(), rhs.end());
		}

		//赋值
		vector& operator=(const vector& rhs)
		{
			vector temp(rhs.begin(), rhs.end());
			swap(temp);
			return *this;
		}

		//析构函数
		~vector()
		{
			if (begin() != end())
			{
				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(), capacity());
			}
		}

		//容器状态
		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() { return size_type(end() - begin()); }
		size_type capacity() { return size_type(end_of_storage - begin()); }
		bool empty() { return begin() == end(); }

		//*************  元素操作
		//增
		void push_back(const_reference x)
		{
			insert(finish, 1, x);
		}

		iterator insert(iterator pos, const_reference x)
		{
			insert(pos, 1, x);
			return pos;
		}
		iterator insert(iterator pos, size_type n, const_reference x)
		{
			if (finish+n-1 != end_of_storage )
			{
				if (pos == finish)
				{
					finish = uninitialized_fill_n(finish, n, x);
				}
				else
				{
					backward_copy(pos, finish, finish+n);
					uninitialized_fill_n(pos, n, x);
					finish = finish + n;
				}
			}
			else
			{
				const size_type old_cap = capacity();
				const size_type new_cap = old_cap != 0 ? 2 * (old_cap+n) : 1;
				iterator new_start = data_allocator.allocate(new_cap);
				iterator new_finish = new_start;

				new_finish = uninitialized_copy(start, pos, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(pos, finish, new_finish);

				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(), capacity());
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_cap;
			}
			return pos;
		}
		/*template<Input_iter>
		iterator insert(iterator pos, Input_iter first, Input_iter last)
		{
			
		}*/

		//删
		void pop_back()
		{
			--finish;
			data_allocator.destroy(finish);
			if (finish == start)
			{
				start = finish = end_of_storage = nullptr;
			}
		}
		//查
		reference operator[](size_type n) { return *(begin() + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		//改
		void swap(vector& rhs)
		{
			iterator start_temp = rhs.start;
			iterator finish_temp = rhs.finish;
			iterator end_temp = rhs.end_of_storage;

			start = rhs.start;
			finish = rhs.finish;
			end_of_storage = rhs.end_of_storage;

			rhs.start = start_temp;
			rhs.finish = finish_temp;
			rhs.end_of_storage = end_temp;
		}
		
	private:
		template <typename Input_iter>
		void range_init(Input_iter first, Input_iter last)
		{
			start = data_allocator.allocate(size_type(last - first));
			finish = end_of_storage = stl::uninitialized_copy(first, last, start);
		}
		

	protected:
		stl::allocator<int> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;

		
		
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

		
	};

	

}





#endif