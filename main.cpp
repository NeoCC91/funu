#include <iostream>

#include "src/base/mat.h"

int main()
{
	funu::Mat<float,6,6>::DataType test;
	for (int i = 0; i < 6; ++i)
	{
		test[i][i]=1;
	}
	funu::Mat<float,6,6> mat(test);
	std::cout<<mat.determinant();
	
	return 0;
}
