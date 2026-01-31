#include "argparser.hpp"


std::string parse_args(const int argc, char const* argv[])
{
	argparse::ArgumentParser parser;
	parser.add_argument<std::string>("--config-file", "-cf")
		.help("config file")
		.default_value("benchmarker/config.json");
	try
	{
		parser.parse_args(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::println(std::cerr, "{}", e.what());
		std::exit(1);
	}

	return {
		parser.get<std::string>("--config-file"),
	};
}