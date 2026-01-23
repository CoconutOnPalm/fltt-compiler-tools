/*
 * Virtual machine converted into a debugger for FLTT2025 project
 * 
 * Kod maszyny wirtualnej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * 
 * Modified by Adam Kostrzewski
*/
#include <iostream>

#include <utility>
#include <vector>
#include <map>
#include <filesystem>
#include <argparse/argparse.hpp>
#include <print>
#include <KEUL/KEUL.hpp>

#include "colors.hpp"
#include "instructions.hpp"

extern void run_parser(std::vector<std::pair<int, var_t>>& program, FILE* data);
extern void run_machine(std::vector<std::pair<int, var_t>>& program, std::array<var_t, 8>& r, std::map<var_t, var_t>& pam, std::vector<std::string>& instructions, std::span<var_t> cin);


std::pair<std::filesystem::path, std::string> parse_args(const int argc, char const* argv[])
{
	argparse::ArgumentParser parser;
	parser.add_argument<std::string>("file")
		.help("input .mr file")
		.required();
	parser.add_argument<std::string>("--input", "-i")
		.help("stdin passed to program");

	try
	{
		parser.parse_args(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::println(std::cerr, "{}", e.what());
	}

	std::string input = (parser.is_used("--input") ? parser.get<std::string>("--input") : "");

	return {
		parser.get<std::string>("file"),
		input,
	};
}


void parse(std::vector<std::pair<int, var_t>>& program, const std::string_view filename)
{
	FILE* data = fopen(filename.data(), "r");
	if( !data ) {
		std::println(std::cerr, "{}Error: could not open '{}'{}", cRed, filename, cReset);
		std::exit(-1);
	}

	run_parser(program, data);

	fclose(data);
}



int main(const int argc, char const * argv[]) {
	std::vector<std::pair<int, var_t>> program;

	auto [filename, console_in] = parse_args(argc, argv);

	
	parse(program, filename.string());
	
	ke::FileReader instr_file(filename);
	std::vector<std::string> instructions = instr_file.readAll();

	std::array<var_t, 8> registers;
	std::map<var_t, var_t> memory;

	std::vector<var_t> cin = ke::splitString<var_t>(console_in, {" "}, [](const std::string& str){ return ke::fromString<var_t>(str).value_or(0); });

	run_machine(program, registers, memory, instructions, cin);

	return 0;
}
