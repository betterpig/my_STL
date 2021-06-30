#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

namespace stl
{


template<typename T, T v>
struct integral_constant
{
	static constexpr T value = v;
};

template<bool b>
using bool_constant = integral_constant<bool, b>;

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

}

#endif