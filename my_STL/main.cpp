#include "allocator.h"
#include "vector.h"
#include <iostream>

using namespace stl;
int main()
{
	vector<int> v({ 1,2,3 });
	v.push_back(4);
	v.insert(v.end() - 2,3, 5);
	for (int i = 0; i < v.size(); i++)
		std::cout << v[i] << std::endl;
	int a = 1;
}