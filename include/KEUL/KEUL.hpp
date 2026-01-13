/** \file Utility
 * 
 * This is a file that includes all Utility Module files.
 * This is, however not a c++ module, but an old c-style include-all file.
 * To get c++20 module functionality, see [TODO: make modules :) ]
 * 
 * StringLib.hpp: 	provides string operations like splitting, trimming, pattern matching etc.
 * Clock.hpp:		provides simple Clock class for measuring time
 * Benchmark.hpp: 	based on Clock.h, provides even simpler Benchmarking class for fast program performance measuring
 * Random.hpp:		provides class that handles pseudo-number generator, a wrapper around c++ std <random>
 * Range.hpp:		simple range structure, mainly to check if a val is within range (a, b) or [a, b]
 * FileSystem:		provides I/O and IO classes that handle specific text file structuring (similar but worse than JSON)
 * 
 */

#pragma once

#include "Error/ErrorDef.hpp"
#include "FileSystem/FileReader.hpp"
#include "Internal/EngineLogging.hpp"
#include "Logging/Logger.hpp"
#include "Logging/Print.hpp"
#include "Logging/SimpleLogger.hpp"
#include "Misc/Assert.hpp"
#include "Misc/Random.hpp"
#include "String/Format.hpp"
#include "String/StringLib.hpp"
#include "Time/LoopBenchmark.hpp"
#include "Time/Timer.hpp"

