#pragma once

#include <ftxui/dom/elements.hpp>


inline ftxui::Element progressbar(float* const progress, std::string* const last_completed)
{
	// ke::restrainVariable(*progress, ke::ClosedRange<float>(0.0, 1.0));
	auto bar = ftxui::gauge(*progress);	

	uint8_t percentile = (*progress) * 100;

	return ftxui::hbox({
		ftxui::text(std::format("{}% ", percentile)) | ftxui::size(ftxui::WIDTH, ftxui::Constraint::EQUAL, 5),
		ftxui::text("[") | ftxui::color(ftxui::Color::GrayDark),
		bar | ftxui::size(ftxui::WIDTH, ftxui::Constraint::EQUAL, 100),
		ftxui::text("] ") | ftxui::color(ftxui::Color::GrayDark),
		ftxui::text((last_completed == nullptr) ? "" : *last_completed) | ftxui::color(ftxui::Color::Cyan)
	});
}