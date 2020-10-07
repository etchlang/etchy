#include <etch/parser.hpp>
#include <iostream>
#include <string>

void repl(std::istream &s) {
	std::cout << "> ";

	std::string input;
	while(std::getline(s, input)) {
		auto ast = etch::parse(input);

		std::cout << "output = ";
		dump(std::cout, ast);
		std::cout << std::endl;

		std::cout << "> ";
	}
}

int main(int, char *[]) {
	repl(std::cin);
	return 0;
}
