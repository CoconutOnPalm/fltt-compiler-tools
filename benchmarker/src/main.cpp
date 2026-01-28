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
#include <utility>
#include <vector>
#include <map>
#include <filesystem>
#include <print>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <argparse/argparse.hpp>
#include <nlohmann/json.hpp>
#include <subprocess.hpp>
#include <KEUL/KEUL.hpp>

#include "../global/colors.hpp"
#include "../global/instructions.hpp"

#include "input/argparser.hpp"
#include "input/jsonparser.hpp"
#include "tui/benchmark_ui.hpp"


extern void run_parser(std::vector<std::pair<int, var_t>>& program, FILE* data);
extern var_t run_machine(std::vector<std::pair<int, var_t>>& program, const std::vector<var_t>& cin);

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
	
	std::string filename = parse_args(argc, argv);
	Config config = parse_config(filename);

	if (!std::filesystem::exists(config.compiler_exe_path))
	{
		std::println("compiler binary '{}' not found", config.compiler_exe_path.string());
	}
	
	auto programs = getBenchmarks(config);

	std::vector<BenchmarkResult> results;
	
	for (const auto& benchmark_unit : programs)
	{
		BenchmarkResult result;
		result.filename = benchmark_unit.lang_filename;
		result.reference_cost = benchmark_unit.reference_cost;
		result.compilation_success = true;
		result.error_message = "";
		
		// launch compilation process
		subprocess::CompletedProcess process = subprocess::run(
			{std::filesystem::path("." / config.compiler_exe_path).string(), benchmark_unit.lang_filename.string(), benchmark_unit.asm_filename.string()},
			subprocess::RunBuilder().cout(subprocess::PipeOption::pipe).cerr(subprocess::PipeOption::pipe)
		);

		if (process.returncode != 0)
		{
			// compilation error
			result.compilation_success = false;
			result.error_message = "Compiler returned code: " + std::to_string(process.returncode);
			result.new_cost = -1;
		}
		else
		{
			try {
				std::vector<std::pair<int, var_t>> program;
				parse(program, benchmark_unit.asm_filename.string());
				result.new_cost = static_cast<uint64_t>(run_machine(program, benchmark_unit.input));
			}
			catch (const std::exception& e) {
				result.compilation_success = false;
				result.error_message = "Runtime error: " + std::string(e.what());
				result.new_cost = -1;
			}
		}
		
		results.push_back(result);
	}
	
	// Show the FTXUI interface
	bool should_override = tui::showBenchmarkResults(results);
	
	if (should_override) 
	{
		overrideCosts(config, results);
	}
	return 0;
}
