#include "allocator.h"
#include "vector.h"
#include <iostream>

using namespace stl;
int main()
{
	vector<int> v({ 1,2,3 });
	v.push_back(4);
	vector<int> v2({ 5,6,7 });
	v.insert(v.end()-2,v2.begin(),v2.end()-1);
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
		std::cout << *it << std::endl;
	int a = 1;
}