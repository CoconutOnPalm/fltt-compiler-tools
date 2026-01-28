#include "benchmark_ui.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "../../../global/colors.hpp"

namespace tui {

ftxui::Element createCostGauge(const std::string& filename, uint64_t ref_cost, uint64_t actual_cost, uint64_t max_cost, int screen_width) {
	const int total_width = std::max(20, screen_width - 60); // Leave space for labels and borders, minimum 20
	const int middle_point = total_width / 2;
	
	// Calculate how much the actual cost deviates from reference
	float cost_ratio = static_cast<float>(actual_cost) / ref_cost;
	
	ftxui::Element gauge_bar;
	if (actual_cost <= ref_cost) {
		// Green gauge from left - when at budget, fill entire left side
		float green_ratio;
		if (actual_cost == ref_cost) 
			green_ratio = 1.0f; // Fill entire left side when exactly at budget 
		else 
			green_ratio = 1.0f - cost_ratio; // How much under budget we are

		int green_width = static_cast<int>(green_ratio * middle_point);
		
		gauge_bar = ftxui::hbox({
			ftxui::gauge(1.0f) | ftxui::color(ftxui::Color::Green) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, green_width),
			ftxui::text("") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, middle_point - green_width),
			ftxui::text("│") | ftxui::color(ftxui::Color::Blue), // Blue line at reference point
			ftxui::text("") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, middle_point - 1)
		});
	} 
	else 
	{
		// Red gauge: full left side + right side showing surplus
		float surplus_ratio = cost_ratio - 1.0f; // How much over budget
		int red_surplus_width = static_cast<int>(surplus_ratio * middle_point);
		red_surplus_width = std::min(red_surplus_width, middle_point - 1); // Don't exceed right side
		
		gauge_bar = ftxui::hbox({
			ftxui::gauge(1.0f) | ftxui::color(ftxui::Color::Red) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, middle_point),
			ftxui::text("│") | ftxui::color(ftxui::Color::Blue), // Blue line at reference point
			ftxui::gauge(1.0f) | ftxui::color(ftxui::Color::Red) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, red_surplus_width),
			ftxui::text("") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, middle_point - 1 - red_surplus_width)
		});
	}
	
	// Status indicator
	auto status_color = actual_cost <= ref_cost ? ftxui::Color::Green : ftxui::Color::Red;
	
	// Put everything on one line: filename, costs, and gauge bar
	return ftxui::vbox({
		ftxui::hbox({
			ftxui::text(filename) | ftxui::bold | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
			ftxui::text(" Ref: " + ((ref_cost == std::numeric_limits<uint64_t>::max()) ? "inf" : std::to_string(ref_cost))) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15),
			ftxui::text(" New: " + std::to_string(actual_cost)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15),
			ftxui::text(" ") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 2),
			gauge_bar | ftxui::border
		}),
		ftxui::separator()
	});
}

bool showBenchmarkResults(std::vector<BenchmarkResult>& results) {
	// Find max cost for scaling
	uint64_t max_cost = 1;
	for (const auto& result : results) {
		max_cost = std::max(max_cost, std::max(result.reference_cost, result.actual_cost));
	}
	max_cost = static_cast<uint64_t>(max_cost * 1.1); // Add 10% padding
	
	// Get terminal width for responsive gauges
	auto screen_size = ftxui::Terminal::Size();
	int screen_width = screen_size.dimx;
	
	// Print header
	std::cout << std::endl;
	std::cout << "=== Benchmark Results ===" << std::endl;
	std::cout << std::endl;
	
	// Print each gauge
	for (const auto& result : results) {
		ftxui::Element gauge_element;
		
		if (result.compilation_success) {
			gauge_element = createCostGauge(
				result.filename.filename().string(),
				result.reference_cost,
				result.actual_cost,
				max_cost,
				screen_width
			);
		} else {
			gauge_element = ftxui::vbox({
				ftxui::hbox({
					ftxui::text(result.filename.filename().string()) | ftxui::bold | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
					ftxui::text(" COMPILATION FAILED") | ftxui::color(ftxui::Color::Red),
					ftxui::text(" Error: " + result.error_message) | ftxui::color(ftxui::Color::Red)
				}),
				ftxui::separator()
			});
		}
		
		// Create a screen sized for this element only
		auto element_screen = ftxui::Screen::Create(
			ftxui::Dimension::Fit(gauge_element)
		);
		ftxui::Render(element_screen, gauge_element);
		element_screen.Print();
		std::cout << std::endl;
	}
	
	// Calculate and print summary
	int successful = std::count_if(results.begin(), results.end(), 
		[](const BenchmarkResult& r) { return r.compilation_success; });
	int within_budget = std::count_if(results.begin(), results.end(),
		[](const BenchmarkResult& r) { return r.compilation_success && r.actual_cost <= r.reference_cost; });
	
	auto summary = ftxui::hbox({
		ftxui::text("Summary: ") | ftxui::bold,
		ftxui::text(std::to_string(within_budget) + "/" + std::to_string(successful) + " within budget") |
			ftxui::color(within_budget == successful ? ftxui::Color::Green : ftxui::Color::Yellow)
	});
	
	std::cout << std::endl;
	std::cout << "=========================" << std::endl;
	
	auto summary_screen = ftxui::Screen::Create(ftxui::Dimension::Fit(summary));
	ftxui::Render(summary_screen, summary);
	summary_screen.Print();
	
	std::cout << std::endl;
	std::cout << "=========================" << std::endl;
	
	// Simple text prompt for user choice
	std::cout << std::endl;
	std::cout << "Do you want to override the reference costs with the actual costs? (y/N): ";
	std::string response;
	std::getline(std::cin, response);
	
	return (response == "y" || response == "Y" || response == "yes" || response == "Yes");
}

} // namespace tui