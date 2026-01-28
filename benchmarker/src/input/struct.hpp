#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>
#include <string>

#include "../../../global/instructions.hpp"

struct Config
{
	std::filesystem::path benchmarks_dir;
	std::filesystem::path benchmark_table;
	std::filesystem::path compiler_exe_path;
	std::filesystem::path compiled_dir;
};

struct BenchmarkUnit
{
	const std::filesystem::path lang_filename;
	const std::filesystem::path asm_filename;
	const std::vector<var_t> input;
	const uint64_t reference_cost;
};

struct BenchmarkResult
{
	std::filesystem::path filename;
	uint64_t reference_cost;
	uint64_t new_cost;
	bool compilation_success;
	std::string error_message;
};