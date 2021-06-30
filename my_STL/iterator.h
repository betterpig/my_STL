#ifndef MYSTL_ITERATOR_H
#define MYSTL_ITERATOR_H

#include <iterator>
#include "type_traits.h"

namespace stl
{

struct input_iterator_tag{};
struct output_iterator_tag {};
struct forward_iterator_tag:public input_iterator_tag {};
struct bidirectional_iterator_tag:public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};

template <typename Iter>
struct iterator_traits
{
	typedef typename Iter::difference_type		difference_type;
	typedef typename Iter::value_type			value_type;
	typedef typename Iter::pointer				pointer;
	typedef typename Iter::reference			reference;
	typedef typename Iter::iterator_category	iterator_category;
};

template <typename T>
struct iterator_traits<T*>
{
	typedef std::ptrdiff_t					difference_type;
	typedef T								value_type;
	typedef T*								pointer;
	typedef T&								reference;
	typedef random_access_iterator_tag		iterator_category;
};

template <typename T>
struct iterator_traits<const T*>
{
	typedef std::ptrdiff_t					difference_type;
	typedef T								value_type;
	typedef T*								pointer;
	typedef T&								reference;
	typedef random_access_iterator_tag		iterator_category;
};

template<typename T>
struct has_iterator_cat
{
private:
	struct two { char a; char b; };
	template<typename T> static two test();
	template<typename T> static char test(typename T::iterator_category* = 0);
public:
	static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template<typename Input_iter,typename tag,bool = has_iterator_cat<iterator_traits<Input_iter>>::value>
struct has_iterator_cat_of:public bool_constant<std::is_convertible<typename iterator_traits<Input_iter>::iterator_category,tag>::value>
{

};

template<typename Input_iter,typename tag>
struct has_iterator_cat_of<Input_iter,tag,false>:public false_type
{

};

template<typename Input_iter>
struct is_input_iterator:public has_iterator_cat_of<Input_iter,input_iterator_tag> {};
template<typename Input_iter>
struct is_output_iterator :public has_iterator_cat_of<Input_iter, output_iterator_tag> {}; 
template<typename Input_iter>
struct is_forward_iterator :public has_iterator_cat_of<Input_iter, forward_iterator_tag> {};
template<typename Input_iter>
struct is_bidirectional_iterator :public has_iterator_cat_of<Input_iter, bidirectional_iterator_tag> {};
template<typename Input_iter>
struct is_random_access_iterator :public has_iterator_cat_of<Input_iter, random_access_iterator_tag> {};





}




#endif