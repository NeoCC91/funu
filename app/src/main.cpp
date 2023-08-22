#include <iostream>
#include <test.h>
#include <mesh/surface_mesh.h>

int main()
{
	funu::surface_mesh sm;
	sm.points_.push_back(funu::Vec4f::Zero());
	std::cout<<sm.points_.front().x()<<std::endl;
	return 0;
}