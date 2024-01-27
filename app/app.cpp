#include <iostream>
#include <filesystem>
#include <format>

int main()
{
	std::filesystem::path p(R"(.)");
	std::cout<<absolute(p).string();
	is_regular_file(p);
	std::filesystem::directory_entry d(p);
	std::cout<<d.exists()<<"\n";
	std::cout<<d.is_regular_file();
	is_regular_file(d);
	space(p);
	
	return 0;
}
