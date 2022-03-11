/*******************************************************************************
*
*    Copyright (c) 2022, SunMinghui<sunminghuisx.com> All rights reserved.
*    Description:
*        A lightweight program performance analyzer class in C/C++. Feasible for simple profiling. Compatible with Mac, Linux and Windows.
*    Edition:
*        0.1    SunMinghui    20220310    Build the initial version
*
*
*******************************************************************************/

#include "performance_analyzer.h"
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#define FUN_NAME_MAX_SIZE 25
#define TIME_MAX_SIZE 25
#define CALL_MAX_SIZE 15
#define PERCENTAGE_MAX_SIZE 15


#ifdef WIN32

#include <Windows.h>

static LARGE_INTEGER frequency; // ticks per second

void init_timer( void )
{
	QueryPerformanceFrequency(&frequency);
}

double get_current_time() 
{
	static LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return 1000.0 * t.QuadPart /frequency.QuadPart;
}

#else

#include <sys/time.h>
void init_timer( void ){ }

double get_current_time() 
{
	static timeval t;
    gettimeofday(&t, NULL);
	return 1.0 * t.tv_sec * 1000.0  // sec to ms
         + 1.0 * t.tv_usec / 1000.0; // us to ms
}

#endif

performance_analyzer::performance_analyzer()
{
	init_timer();
	whole_program.begin_time = get_current_time();
	whole_program.count = 1;
}

performance_analyzer::~performance_analyzer()
{
	
}

void performance_analyzer::begin( const std::string& func_name ) 
{
	std::unordered_map<std::string, sd_timer_data>::iterator it = instance().timer_data.find(func_name);
	if( it == instance().timer_data.end() ) 
	{
		instance().timer_data.insert( std::pair<std::string, sd_timer_data>( func_name, sd_timer_data()) );
		it = instance().timer_data.find(func_name);
	}
	it->second.begin_time = get_current_time();
}

void performance_analyzer::end( const std::string& func_name ) 
{
	std::unordered_map<std::string, sd_timer_data>::iterator it = instance().timer_data.find( func_name );
	if( it != instance().timer_data.end() ) 
	{
		it->second.total_run_time += get_current_time() - it->second.begin_time;
		it->second.count++;
	} 
	else 
	{
		std::cerr << "Error: This function '" << func_name << "' is not defined" << std::endl;
		system( "pause" );
	}
}

std::string performance_analyzer::summery( void ) 
{
	performance_analyzer::instance().whole_program.total_run_time = get_current_time() - performance_analyzer::instance().whole_program.begin_time;

	std::stringstream ss;

	static const std::string func_name = "Function Name";
	static const int func_name_size = std::max((int)func_name.length(), FUN_NAME_MAX_SIZE);
	static const std::string total_time = "Total";
	static const int total_time_size = std::max((int)total_time.length(), TIME_MAX_SIZE);
	static const std::string called_times = "Be Called";
	const int called_times_size = std::max((int)called_times.length(), CALL_MAX_SIZE);
	static const std::string percentage   = "Percentage";
	const int percentage_size = std::max((int)percentage.length(),   PERCENTAGE_MAX_SIZE);

	ss << "------------------------------------------------------------------------------------------" << std::endl;
	ss << "| Profiling Summery ..." << std::endl;
	ss << "------------------------------------------------------------------------------------------" << std::endl;
	ss << "| ";
	
	ss.width(func_name_size);
	ss << std::left << func_name;
	ss << " |";
	ss.width(total_time_size);
	ss << std::right << total_time;
	ss << " |";
	ss.width(called_times_size);
	ss << std::right << called_times;
	ss << " |";
	ss.width(percentage_size);
	ss << std::right << percentage;
	ss << " |" << std::endl;

	double tatol_running_time = performance_analyzer::instance().whole_program.total_run_time;

	ss << "| ";
	ss.width( func_name_size );
	ss << std::left << "Total Run Time" << " |";
	ss.width( total_time_size - sizeof("ms") );
	ss << std::right << tatol_running_time << " ms |";
	ss.width( called_times_size - sizeof("times") );
	ss << std::right << 1 << " times |";
	ss.width( percentage_size - sizeof("%") );
	ss << std::right << 100.000 << " % |" << std::endl;

	std::unordered_map<std::string, sd_timer_data>::iterator it;
	for( it = instance().timer_data.begin(); it != instance().timer_data.end(); it++ ) 
	{
		ss << "| ";
		ss.width( func_name_size );
		ss << std::left << it->first << " |";
		ss.width( total_time_size - sizeof("ms") );
		ss << std::right<< it->second.total_run_time << " ms |";
		ss.width( called_times_size - sizeof("times") );
		ss << std::right<< it->second.count << " times |";
		ss.width( percentage_size - sizeof("%") );
		ss << std::right<< int( 100000 * it->second.total_run_time / tatol_running_time ) / 1000.0 << " % |" << std::endl;
	}

	return ss.str();
}


