#pragma once

#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "../input/struct.hpp"

namespace tui {

// Create a cost gauge element for a single benchmark result
ftxui::Element createCostGauge(const std::string& filename, uint64_t ref_cost, uint64_t actual_cost, uint64_t max_cost, int screen_width);

// Show the benchmark results interface and return whether user wants to override costs
bool showBenchmarkResults(std::vector<BenchmarkResult>& results);

} // namespace tui