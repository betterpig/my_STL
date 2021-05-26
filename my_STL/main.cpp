#include "allocator.h"
#include "vector.h"

using namespace stl;
int main()
{
	vector<int> v;
	v.push_back(2);
	v.push_back(3);
	vector<int> v2(v.begin(), v.end());
	int a = 1;
}