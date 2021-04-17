#include <boost/program_options.hpp>
#include <etch/analysis/dump.hpp>
#include <etch/compiler.hpp>
#include <fstream>
#include <iostream>
#include <string>

void repl(std::istream &s) {
	std::cout << "> ";

	std::string input;
	while(std::getline(s, input)) {
		auto ast = etch::compile(input);
		dump(std::cout, ast) << std::endl;
		std::cout << "> ";
	}
}

int main(int argc, char *argv[]) {
	namespace po = boost::program_options;

	po::options_description desc("Options");
	desc.add_options()
		("input-file",    po::value<std::vector<std::string>>(), "Add input file")
		("output-file,o", po::value<std::string>(),              "Set output file")
		("help,h",                                               "Display help message")
		("verbose,v",     po::bool_switch(),                     "Enable verbose output")
		("interactive,i", po::bool_switch(),                     "Run interactive session")
	;

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if(vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}

	if(vm["interactive"].as<bool>()) {
		repl(std::cin);
		return 0;
	}

	if(!vm.count("input-file")) {
		std::cerr << argv[0] << ": No input files specified" << std::endl;
		std::cout << desc << std::endl;
		return 0;
	}

	auto inputs = vm["input-file"].as<std::vector<std::string>>();

	for(auto &input : inputs) {
		if(vm["verbose"].as<bool>()) {
			std::cout << "processing " << input << std::endl;
		}

		std::ifstream f(input, std::ios::in | std::ios::ate);
		if(f.fail()) {
			std::cerr << argv[0] << ": " << input << ": failed to open file" << std::endl;
			return 1;
		}

		size_t filelen = f.tellg();

		f.seekg(0, std::ios::beg);

		std::string contents;
		contents.resize(filelen);

		f.read(contents.data(), filelen);

		auto ast = etch::compile(contents);
		dump(std::cout, ast) << std::endl;
	}

	return 0;
}
