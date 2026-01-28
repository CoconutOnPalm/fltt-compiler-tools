#include "jsonparser.hpp"

#include <map>
#include <fstream>
#include <KEUL/KEUL.hpp>

using json = nlohmann::json;


Config parse_config(const std::string_view config_path)
{
	if (!std::filesystem::exists(config_path))
	{
		KE_LOGERROR("file '{}' does not exist", config_path);
		std::exit(1);
	}

	std::ifstream ifstr(std::filesystem::path{config_path});
	json data = json::parse(ifstr);

	std::filesystem::path benchmarks_path;
	std::filesystem::path benchmark_table;
	std::filesystem::path compiler_exe_path;
	std::filesystem::path compiled_path;

	try {
		benchmarks_path = data["benchmarks-dir"].get<std::string>();
		benchmark_table = data["benchmark-table"].get<std::string>();
		compiler_exe_path = data["compiler-exe"].get<std::string>();
		compiled_path = data["compiled-dir"].get<std::string>();
	} catch (std::exception& e) {
		std::println(std::cerr, "{}", e.what());
		std::exit(1);
	}

	return Config {
		.benchmarks_dir = benchmarks_path,
		.benchmark_table = benchmark_table,
		.compiler_exe_path = compiler_exe_path,
		.compiled_dir = compiled_path,
	};
}

std::vector<BenchmarkUnit> getBenchmarks(const Config& config)
{
	const auto& dir_prefix = config.benchmarks_dir;
	const auto& compiled_prefix = config.compiled_dir;
	const auto& table = config.benchmark_table;

	std::ifstream ifstr(table);
	json data = json::parse(ifstr);
	std::vector<BenchmarkUnit> result;
    for (const auto &entry : data) 
	{
        // skip malformed entries
        if (!entry.contains("file")) 
			continue;
        const std::string file = entry["file"].get<std::string>();
		
		std::vector<var_t> inputs;
		if (entry.contains("in") && entry["in"].is_array()) 
		{
            for (const auto &v : entry["in"]) 
				inputs.push_back(v.get<var_t>());
        }

		uint64_t cost = std::numeric_limits<uint64_t>::max();
		if (entry.contains("cost"))
			cost = entry["cost"].get<uint64_t>();
		
		result.emplace_back(BenchmarkUnit {
			.lang_filename = dir_prefix / file,
			.asm_filename = std::filesystem::path(compiled_prefix / file).replace_extension("mr"),
			.input = inputs,
			.reference_cost = cost
		});

		const auto& last = result.back();
    }

	return result;
}

void overrideCosts(const Config& config, const std::vector<BenchmarkResult>& results)
{
	if (!std::filesystem::exists(config.benchmark_table))
	{
		KE_LOGERROR("benchmark table '{}' does not exist", config.benchmark_table.string());
		return;
	}

	// Read current benchmark table
	std::ifstream ifstr(config.benchmark_table);
	json data = json::parse(ifstr);
	ifstr.close();

	// Create a map of filenames to new costs for quick lookup
	std::map<std::string, uint64_t> new_costs;
	for (const auto& result : results)
	{
		if (result.compilation_success)
		{
			new_costs[result.filename.filename().string()] = result.actual_cost;
		}
	}

	// Update costs in JSON data
	for (auto& entry : data)
	{
		if (entry.contains("file"))
		{
			const std::string filename = entry["file"].get<std::string>();
			if (new_costs.find(filename) != new_costs.end())
			{
				entry["cost"] = new_costs[filename];
			}
		}
	}

	// Write updated data back to file
	std::ofstream ofstr(config.benchmark_table);
	if (!ofstr)
	{
		KE_LOGERROR("could not write to '{}'", config.benchmark_table.string());
		return;
	}

	std::println(ofstr, "{}", data.dump(4));
}