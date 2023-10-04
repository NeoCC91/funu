#include <iostream>
#include <format>
#include <limits>
#include <string>

class TextHolder
{
public:
	TextHolder(std::string text) : m_text{std::move(text)}
	{
	}

	std::string getText() { return "guoce"; }

private:
	std::string m_text;
};

int main()
{
	std::cout << TextHolder{ "Helloxx world!" }.getText() << std::endl;
	//std::cout << std::move(textHolder).getText() << std::endl;
	return 0;
}