#include <etch/parser.hpp>
#include <iostream>

int main(int, char *[]) {
	std::string input;
	std::cin >> input;

	auto ast = etch::parse(input);

	std::cout << ast << std::endl;

	return 0;
}
