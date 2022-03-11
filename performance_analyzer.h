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

#ifndef __PERFORMANCE_ANALYZER_H__
#define __PERFORMANCE_ANALYZER_H__

#include <string>
#include <unordered_map>

class performance_analyzer 
{	
	public:
		static void begin(const std::string& function_name = "anonymous function");
		static void end(const std::string& function_name   = "anonymous function"); 
		static std::string summery(void); 

	private:
		performance_analyzer();
		~performance_analyzer();
		
		inline static performance_analyzer& instance()
		{
			static performance_analyzer performance_analyzer_instance; 
			return performance_analyzer_instance; 
		}

	private:
		struct sd_timer_data 
		{
			double begin_time; 
			double total_run_time; 
			int count; 
			sd_timer_data() : begin_time( 0.0 ), total_run_time( 0.0 ), count(0) { } 
		}; 
		sd_timer_data whole_program; 
		std::unordered_map<std::string, sd_timer_data> timer_data; 
}; 

#endif // __PERFORMANCE_ANALYZER_H__