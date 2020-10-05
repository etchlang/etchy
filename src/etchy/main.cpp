#include <etch/parser.hpp>
#include <iostream>
#include <string>

int main(int, char *[]) {
	std::string input;
	while(std::getline(std::cin, input)) {
		std::cout << "input = " << input << std::endl;
		auto ast = etch::parse(input);
		std::cout << "output = " << ast << std::endl;
	}
	return 0;
}
