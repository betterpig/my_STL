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
	template<typename Input_iter, typename Forward_iter>
	Forward_iter forward_copy(Input_iter first, Input_iter last, Forward_iter dfirst)
	{
		while (first != last)
			*(dfirst++) = *(first++);
		return dfirst;
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
		vector():start(nullptr),finish(nullptr),end_of_storage(nullptr) {  }//默认构造函数

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
		vector& operator=(std::initializer_list<T> init)
		{
			vector temp(init.begin(), init.end());
			swap(temp);
			return *this;
		}
		void assign(size_type n, const_reference x)
		{
			if (n > capacity())
			{
				vector temp(n, x);
				swap(temp);
			}
			else if (n > size())
			{
				uninitialized_fill_n(begin(), n, x);
			}
			else
			{
				iterator new_finish = uninitialized_fill_n(begin(), n, x);
				erase(new_finish, finish);
				finish = new_finish;
			}
		}
		template<typename Input_iter, typename std::enable_if<is_input_iterator<Input_iter>::value,int>::type = 0>
		void assign(Input_iter first, Input_iter last)
		{
			size_type n = last - first;
			if (n > capacity())
			{
				vector temp(first,last);
				swap(temp);
			}
			else if (n > size())
			{
				uninitialized_copy(first,last,start);
			}
			else
			{
				iterator new_finish = uninitialized_copy(first, last, start);
				erase(new_finish, finish);
				finish = new_finish;
			}
		}
		void assign(std::initializer_list<value_type> init)
		{
			assign(init.begin(), init.end());
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

		//容量相关
		void resize(size_type n)
		{
			resize(n, value_type());
		}

		void resize(size_type n, const value_type& x)
		{
			if (n < size())
			{
				for (iterator it = begin() + n; it != end(); it++)
					data_allocator.destroy(it);
				finish = begin() + n;
			}
			else if(n>=size() && n <= capacity())
			{
				for (iterator it = end(); it != begin() + n; it++)
					data_allocator.construct(it,x);
				finish = begin()+n;
			}
			else
			{
				iterator new_start = data_allocator.allocate(n);
				iterator new_finish = new_start;

				new_finish = uninitialized_copy(start, finish, new_start);
				new_finish = uninitialized_fill_n(new_finish, n-size(), x);

				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(), capacity());
				start = new_start;
				finish = new_finish;
				end_of_storage = start + n;
			}
		}

		void reserve(size_type new_cap)
		{
			if (new_cap > capacity())
			{
				iterator new_start = data_allocator.allocate(new_cap);
				iterator new_finish = new_start;

				new_finish = uninitialized_copy(start, finish, new_start);

				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(), capacity());
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_cap;
			}
		}
		//void shrink_to_fit()

		//*************  元素操作
		//增
		void push_back(const_reference x)
		{//通过强制类型转换，使其匹配到insert(iterator,size_type,const_reference)重载函数，否则会匹配到insert(iterator,Input_iter,Input_iter)函数
			insert(finish, size_type(1), x);
		}
		iterator insert(iterator pos, const_reference x)
		{
			insert(pos, size_type(1), x);
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
		template<typename Input_iter>
		iterator insert(iterator pos, Input_iter first, Input_iter last)
		{
			size_type n = size_type(last - first);
			if (finish + n - 1 != end_of_storage)
			{
				if (pos == finish)
				{
					int a = 1;
					finish = uninitialized_copy(first, last, finish);
				}
				else
				{
					backward_copy(pos, finish, finish + n);
					uninitialized_copy(first, last, pos);
					finish = finish + n;
				}
			}
			else
			{
				const size_type old_cap = capacity();
				const size_type new_cap = old_cap != 0 ? 2 * (old_cap + n) : 1;
				iterator new_start = data_allocator.allocate(new_cap);
				iterator new_finish = new_start;

				new_finish = uninitialized_copy(start, pos, new_start);
				new_finish = uninitialized_copy(first, last, new_finish);
				new_finish = uninitialized_copy(pos, finish, new_finish);

				data_allocator.destroy(begin(), end());
				data_allocator.deallocate(begin(), capacity());
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_cap;
			}
			return pos;
		}
		iterator insert(iterator pos, std::initializer_list<T> init)
		{
			insert(pos, init.begin(), init.end());
			return pos;
		}

		//删
		void pop_back()
		{
			if (!empty())
				erase(finish-1,finish);
		}
		iterator erase(iterator pos)
		{
			erase(pos, pos + 1);
			return pos;
		}
		iterator erase(iterator range1, iterator range2)
		{
			for (iterator it = range1; it != range2; it++)
				data_allocator.destroy(it);
			finish = forward_copy(range2, finish, range1);
			return range1;
		}
		void clear()
		{
			for (iterator it = start; it != finish; it++)
				data_allocator.destroy(it);
		}

		//查
		reference operator[](size_type n) { return *(begin() + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		reference at(size_type n) 
		{ 
			if (n < size())
				return *(begin() + n);
			else
				return -1;
		}

		//改
		void swap(vector& rhs)
		{
			iterator start_temp = start;
			iterator finish_temp = finish;
			iterator end_temp = end_of_storage;

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