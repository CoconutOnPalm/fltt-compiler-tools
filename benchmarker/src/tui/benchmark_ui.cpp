#include "benchmark_ui.hpp"

#include <algorithm>
#include <set>
#include <print>
#include <string>
#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <KEUL/KEUL.hpp>

#include "../../../global/colors.hpp"

namespace tui
{

	ftxui::Element createCostGauge(const std::string& filename, uint64_t ref_cost, uint64_t new_cost, uint64_t max_cost, int screen_width)
	{
		const int width = std::max(20, screen_width - 60); // Leave space for labels and borders, minimum 20
		const int halfwidth = width / 2;

		// Calculate how much the actual cost deviates from reference
		float cost_ratio = static_cast<float>(new_cost) / ref_cost;
		float surplus_ratio = cost_ratio - 1.0f; // How much over budget
		ke::restrainVariable(cost_ratio, ke::ClosedRange(0.0f, 1.0f));
		ke::restrainVariable(surplus_ratio, ke::ClosedRange(0.0f, 1.0f));

		ftxui::Element gauge_bar;

		const ftxui::Color left_color = (new_cost < ref_cost) ? ftxui::Color::Green : ftxui::Color::White;
		
		KE_LOGDEBUG("cost-ratio={}", cost_ratio);
		
		gauge_bar = ftxui::hbox({
			ftxui::gauge(cost_ratio) | ftxui::color(left_color) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, halfwidth),
			ftxui::text("│") | ftxui::color(ftxui::Color::Blue), // Blue line at reference point
			ftxui::gauge(surplus_ratio) | ftxui::color(ftxui::Color::Red) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, halfwidth),
			// ftxui::text("") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, halfwidth - (halfwidth * surplus_ratio - 1))
		});

		// Status indicator
		auto status_color = new_cost <= ref_cost ? ftxui::Color::Green : ftxui::Color::Red;

		// Put everything on one line: filename, costs, and gauge bar
		return ftxui::vbox({
			ftxui::hbox({
				ftxui::text(filename) | ftxui::bold | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30),
				ftxui::text(" Ref: " + ((ref_cost == std::numeric_limits<uint64_t>::max()) ? "inf" : std::to_string(ref_cost))) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
				ftxui::text(" New: " + std::to_string(new_cost)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
				ftxui::text(" ") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 2),
				gauge_bar
			}),
			ftxui::separator()
			});
	}

	bool showBenchmarkResults(std::vector<BenchmarkResult>& results)
	{
		// Find max cost for scaling
		uint64_t max_cost = 1;
		for (const auto& result : results)
		{
			max_cost = std::max(max_cost, std::max(result.reference_cost, result.new_cost));
		}
		max_cost = static_cast<uint64_t>(max_cost * 1.1); // Add 10% padding

		// Get terminal width for responsive gauges
		auto screen_size = ftxui::Terminal::Size();
		int screen_width = screen_size.dimx;

		std::println();

		// Print each gauge
		for (const auto& result : results)
		{
			ftxui::Element gauge_element;

			if (result.compilation_success)
			{
				gauge_element = createCostGauge(
					result.filename.filename().string(),
					result.reference_cost,
					result.new_cost,
					max_cost,
					screen_width
				);
			}
			else
			{
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
			[](const BenchmarkResult& r) { return r.compilation_success && r.new_cost <= r.reference_cost; });

		auto summary = ftxui::hbox({
			ftxui::text("Summary: ") | ftxui::bold,
			ftxui::text(std::to_string(within_budget) + "/" + std::to_string(successful) + " within budget") |
				ftxui::color(within_budget == successful ? ftxui::Color::Green : ftxui::Color::Yellow)
			});

		auto summary_screen = ftxui::Screen::Create(ftxui::Dimension::Fit(summary));
		ftxui::Render(summary_screen, summary);
		summary_screen.Print();

		const static std::set<std::string> mapped_responses = {
			"Y", "YES"
		};

		// Simple text prompt for user choice
		std::println();
		std::print("override the reference costs? (y/N): ");
		std::string response;
		std::getline(std::cin, response);

		return mapped_responses.contains(ke::toUpper(response));
	}

} // namespace tui