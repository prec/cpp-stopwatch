/*
This file is part of Stopwatch, a project by Tommaso Urli.

Stopwatch is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Stopwatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Stopwatch.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "stdafx.h"

#ifndef WIN32
	/* The classes below are exported */
	#pragma GCC visibility push(default)
#endif

// Generic stopwatch exception class
struct StopwatchException {
public:
	StopwatchException(std::string error) : error(error) { }
	std::string error;
};


enum StopwatchMode {
	NONE		= 0,	// Clock is not initialized
	CPU_TIME	= 1,	// Clock calculates time ranges using ctime and CLOCKS_PER_SEC
	REAL_TIME	= 2,	// Clock calculates time by asking the operating system how much real time passed
};

/** 
	@brief A static class representing a stopwatch.
	
	The Stopwatch class can be used to profile code performance. Basically one wraps the code to be measured using two method calls:
	
		@code
		Stopwatch::start("Piece of code ID");
		// Hic est code
		Stopwatch::stop("Piece of code ID");
		@endcode
		
	A string representing the code ID is provided so that nested portions of code can be profiled separately:
	
		@code
		Stopwatch::start("Algorithm 1");
		
		// Init
		
		Stopwatch::start("Algorithm 1 - Loop");
		// Loop
		Stopwatch::stop("Algorithm 1 - Loop");
		
		Stopwatch::stop("Algorithm 1");
		@endcode
		
	If the code for a certain task is scattered through different files or portions of the same file one can use the start-pause-stop method:
	
		@code
		Stopwatch::start("Setup");
		// First part of setup
		Stopwatch::pause("Setup");
		
		Stopwatch::start("Main logic");
		// Main logic
		Stopwatch::stop("Main logic");
		
		Stopwatch::start("Setup");
		// Cleanup
		Stopwatch::stop("Setup");
		@endcode
		
	Finally, to report the results of the measurements just run:
	
		@code
		Stopwatch::report("Code ID");
		@endcode
		
	or, to report all the measurements:
	
		@code
		Stopwatch::report_all();
		@endcode
		
	Additionally one can provide the report* methods with a std::ostream (or subclass) to redirect the output (e.g. on a file).
		

*/
class Stopwatch {
public:
	
	/** Initialize stopwatch to use a certain time taking mode */
	static void init(StopwatchMode mode);

	/** Start the stopwatch related to a certain piece of code */
	static void start(std::string perf_name);
		
	/** Stops the stopwatch related to a certain piece of code */
	static void stop(std::string perf_name);
	
	/** Stops the stopwatch related to a certain piece of code */
	static void pause(std::string perf_name);

	/** Reset a certain performance record */
	static void reset(std::string perf_name);
		
	/** Resets all the performance records */
	static void reset_all();
	
	/** Dump the data of a certain performance record */
	static void report(std::string perf_name, std::ostream& output = std::cout);

	/** Dump the data of all the performance records */
	static void report_all(std::ostream& output = std::cout);

	/** Struct to hold the performance data */
	struct PerformanceData {
	
		PerformanceData() :
			clock_start(0),
			total_time(0),
			min_time(0),
			max_time(0),
			stops(0) {
		}
	
		long double	clock_start;
		long double	total_time;
		long double	min_time;
		long double	max_time;
		int	stops;
	};

	/**	Turn off clock, all the Stopwatch::* methods return without doing
	  anything if this method is called. */
	static void turn_off();
	
	/** Turn on clock, restore clock operativity after a turn_off(). */
	static void turn_on();
	
private:

	/** Take time, depends on mode */
	static long double take_time();

	/** Time taking mode */
	static StopwatchMode mode;

	/** Pointer to the dynamic structure which holds the collection of performance data */
	static std::map<std::string, PerformanceData >* records_of;
	
	/** Flag to hold the clock's status */
	static bool active;
};

#ifndef WIN32
	#pragma GCC visibility pop
#endif

#endif
