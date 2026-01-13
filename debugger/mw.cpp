/*
 * Virtual machine converted into a debugger for FLTT2025 project
 * 
 * Kod interpretera maszyny rejestrowej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * (wersja var_t)
 * 
 * Modified by Adam Kostrzewski
*/
#include <iostream>
#include <locale>
#include <print>
#include <format>
#include <functional>
#include <algorithm>

#include <utility>
#include <vector>
#include <map>

#include <cstdlib> 	// rand()
#include <ctime>

// FTXUI
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "instructions.hpp"
#include "colors.hpp"



void run_machine(std::vector<std::pair<int, var_t>>& program, std::array<var_t, 8>& r, std::map<var_t, var_t>& pam, std::vector<std::string>& instructions)
{
	constinit static std::array<std::string, 8> reg_string_mapper = {
		"RA", "RB", "RC", "RD", "RE", "RF", "RG", "RH"
	};
	
	var_t tmp {};
	var_t lr = 0;
	var_t t = 0;
	var_t io {};

	std::srand(std::time(0));
	for(int i = 0; i<8; i++ ) 
		r[i] = rand();

	std::vector<std::string> logs;
	logs.push_back("Debugger started.");

	auto log = [&](const std::string& msg) {
		logs.push_back(msg);
		if (logs.size() > 50) logs.erase(logs.begin());
	};

	std::function<void()> step = [&]() {
		if( lr < 0 || lr >= (int)program.size() ) {
			log(std::format("ERROR: PC out of bounds: {}", lr));
			return;
		}

		if (program[lr].first == HALT) {
			log("Program HALTED.");
			return;
		}

		switch( program[lr].first )
		{
			case READ:		
				log("READ instruction encountered (input mocked as 0)"); 
				r[0] = 0; // consistent mock
				io+=100; lr++; break;
			case WRITE:		
				log(std::format("> Output: {}", r[0])); 
				io+=100; lr++; break;

			case LOAD:		r[0] = pam[program[lr].second]; t+=50; lr++; break;
			case STORE:		pam[program[lr].second] = r[0]; t+=50; lr++; break;
			case RLOAD:		r[0] = pam[r[program[lr].second]]; t+=50; lr++; break;
			case RSTORE:	pam[r[program[lr].second]] = r[0]; t+=50; lr++; break;

			case ADD:		r[0] += r[program[lr].second]; t+=5; lr++; break;
			case SUB:		r[0] -= r[0]>=r[program[lr].second]?r[program[lr].second]:r[0]; t+=5; lr++; break;
			case SWP:		tmp = r[program[lr].second]; r[program[lr].second] = r[0]; r[0] = tmp; t+=5; lr++; break;

			case RST:		r[program[lr].second] = 0; t+=1; lr++; break;
			case INC:		r[program[lr].second]++; t+=1; lr++; break;
			case DEC:		if(r[program[lr].second]>0) r[program[lr].second]--; t+=1; lr++; break;
			case SHL:		r[program[lr].second]<<=1; t+=1; lr++; break;
			case SHR:		r[program[lr].second]>>=1; t+=1; lr++; break;

			case JUMP: 		lr = program[lr].second; t+=1; break;
			case JPOS:		if(r[0] > 0)  lr = program[lr].second; else lr++; t+=1; break;
			case JZERO:		if(r[0] == 0) lr = program[lr].second; else lr++; t+=1; break;
			
			case CALL:		r[0] = lr+1; lr = program[lr].second; t+=1; break;
			case RTRN:		lr = r[0]; t+=1; break;

			default: break;
		}
	};

	auto screen = ftxui::ScreenInteractive::Fullscreen();
	const std::map<std::string, std::function<void()>> command_mapper = {
		{ "n", step },
		{ "next", step },
		{ "q", screen.ExitLoopClosure() },
		{ "quit", screen.ExitLoopClosure() },
	};

	std::string input_buffer;

	// View Components
	auto code_renderer = ftxui::Renderer([&] {
		ftxui::Elements lines;
		int start = std::max(0, (int)lr - 15);
		int end = std::min((int)instructions.size(), start + 30);
		
		for(int i = start; i < end; ++i) {
			auto content = ftxui::text(std::format("{:04} {}", i, instructions[i]));
			if (i == lr) {
				lines.push_back(content | ftxui::bold | ftxui::bgcolor(ftxui::Color::Blue));
			} else {
				lines.push_back(content);
			}
		}
		return ftxui::window(ftxui::text("Instructions"), ftxui::vbox(std::move(lines)));
	});

	auto regs_renderer = ftxui::Renderer([&] {
		ftxui::Elements items;
		for(int i=0; i<8; ++i) {
			items.push_back(ftxui::text(std::format("{} = {}", reg_string_mapper[i], r[i])));
		}
		return ftxui::window(ftxui::text("Registers"), ftxui::vbox(std::move(items)));
	});

	auto mem_renderer = ftxui::Renderer([&] {
		ftxui::Elements items;
		int count = 0;
		if (pam.empty()) items.push_back(ftxui::text("Empty"));
		for(const auto& [addr, val] : pam) {
			items.push_back(ftxui::text(std::format("[{}] = {}", addr, val)));
			if (++count > 20) break;
		}
		return ftxui::window(ftxui::text("Memory"), ftxui::vbox(std::move(items)));
	});

	auto log_renderer = ftxui::Renderer([&] {
		ftxui::Elements items;
		int start = std::max(0, (int)logs.size() - 8);
		for(size_t i = start; i < logs.size(); ++i) {
			items.push_back(ftxui::text(logs[i]));
		}
		return ftxui::window(ftxui::text("Logs"), ftxui::vbox(std::move(items)));
	});

	// Input handling
	ftxui::InputOption input_opt;
	input_opt.multiline = false;
	input_opt.on_enter = [&] {
		auto command_ptr = command_mapper.find(input_buffer);
		if (command_ptr == command_mapper.end())
			log(std::format("unknown command: '{}'", input_buffer));
		else {
			auto callback = command_ptr->second;
			callback();
		}

		input_buffer.clear();
	};
	
	auto input_component = ftxui::Input(&input_buffer, "", input_opt);

	auto layout = ftxui::Container::Vertical({
		input_component,
		ftxui::Container::Horizontal({
			code_renderer,
			ftxui::Container::Vertical({
				regs_renderer,
				mem_renderer,
				log_renderer
			})
		}) | ftxui::flex,
	});

	auto main_renderer = ftxui::Renderer(layout, [&] {
		return ftxui::vbox({
			ftxui::hbox({
				code_renderer->Render() | ftxui::flex,
				ftxui::vbox({
					regs_renderer->Render(),
					mem_renderer->Render() | ftxui::flex,
					log_renderer->Render()
				}) | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 40)
			}) | ftxui::flex,
			ftxui::separator(),
			ftxui::hbox({
				ftxui::text(std::format("Cycle: {} IO: {}", t, io)) | ftxui::border,
				input_component->Render() | ftxui::color(ftxui::Color::White) | ftxui::bgcolor(ftxui::Color::Black) | ftxui::flex
			})
		});
	});

	screen.Loop(main_renderer);

	std::println("{2}Program finished (cost: {3}{0}{2}; incl. i/o: {1}).{4}", t + io, io, cBlue, cRed, cReset);
}

