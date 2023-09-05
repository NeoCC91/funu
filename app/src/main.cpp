#include <iostream>
#include <format>
#include <limits>

int main()
{
	std::cout<<std::format("{}",std::numeric_limits<float>::digits)<<std::endl;
	return 0;
}