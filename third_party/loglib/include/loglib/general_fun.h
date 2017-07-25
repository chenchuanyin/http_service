#ifndef _GENERAL_FUN_GUO_
#define _GENERAL_FUN_GUO_

#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <functional>
#include <exception>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#pragma warning(disable:4996)
#endif


#ifdef __linux
#include <sys/time.h>
#endif

/**************************************************
get_local_day function
@brief return local day
@return string
@"20130709"

***************************************************/
std::string get_local_day();

/**************************************************
get_local_day function
@brief return local minute
@return string
@"2013_07_09_11_12"

***************************************************/
std::string get_local_minute();

/**************************************************
get_local_time function
@brief return time
@return string
@"2013.07.09 14:42:22"

***************************************************/
std::string get_local_time();

/**************************************************
get_local_time_usec function
@brief return time
@return string
@"2013.07.09 14:42:22:888"

***************************************************/
std::string get_local_time_usec();

std::string trim(std::string st);

void general_sleep(long milisecond);

#endif
