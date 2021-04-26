#include <boost/program_options.hpp>
#include <etch/compiler.hpp>
#include <etch/linker.hpp>
#include <fstream>
#include <iostream>
#include <string>

void repl(std::istream &s, bool debug) {
	etch::compiler comp;
	comp.debug = debug;
	comp.tgt = etch::compiler::target::llvm_assembly;

	std::cout << "> ";

	std::string input;
	while(std::getline(s, input)) {
		auto r = comp.run(input);
		std::cout << std::endl;
		std::cout << r << std::endl;
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
		("debug",         po::bool_switch(),                     "Enable debug output")
		("interactive,i", po::bool_switch(),                     "Run interactive session")
		("llvm-asm",      po::bool_switch(),                     "Output LLVM assembly")
		("verbose,v",     po::bool_switch(),                     "Enable verbose output")
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

	bool debug = false;
	if(vm["debug"].as<bool>()) {
		debug = true;
	}

	bool llvm_asm = false;
	if(vm["llvm-asm"].as<bool>()) {
		llvm_asm = true;
	}

	if(vm["interactive"].as<bool>()) {
		repl(std::cin, debug);
		return 0;
	}

	if(!vm.count("input-file")) {
		std::cerr << argv[0] << ": No input files specified" << std::endl;
		std::cout << desc << std::endl;
		return 0;
	}

	auto inputs = vm["input-file"].as<std::vector<std::string>>();
	std::vector<std::string> outputs;

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

		etch::compiler comp(input);
		comp.debug = debug;
		if(llvm_asm) {
			comp.tgt = etch::compiler::target::llvm_assembly;
		}
		auto r = comp.run(contents);

		if(llvm_asm) {
			std::cout << r;
		} else {
			std::string_view sv(input);
			auto pos_dot = sv.find_last_of('.');
			auto basename = sv.substr(0, pos_dot);
			auto output = std::string(basename) + ".o";
			outputs.emplace_back(output);

			std::ofstream of(output, std::ios::out | std::ios::binary | std::ios::trunc);
			if(of.fail()) {
				std::cerr << argv[0] << ": " << "a.out: failed to open output file" << std::endl;
				return 1;
			}

			of << r;
		}
	}

	if(!llvm_asm) {
		etch::linker ld;
		for(auto &output : outputs) {
			ld.push_back(output);
		}
		ld.run();
	}

	return 0;
}
