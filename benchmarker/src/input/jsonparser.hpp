#pragma once

#include <nlohmann/json.hpp>

#include "struct.hpp"

Config parse_config(const std::string_view config_path);

std::vector<BenchmarkUnit> getBenchmarks(const Config& config);

void overrideCosts(const Config& config, const std::vector<BenchmarkResult>& results);