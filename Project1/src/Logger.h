/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
 */
#pragma once

//***************************
// C++ Includes
//***************************

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "Logging.h"

//***************************
// Definition
//***************************

// Logging class object
Logging g_errorLogs;

/*
 * LOG(): log error string
 * 
 * @params:
 *		s - error string to be logged
 */
#define LOG(s) { \
	std::string errorString = "System error"; \
	errorString.append(s); \
	g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
};

 /*
  * CHECK(): check for general error and log, throws exception
  *
  * @params:
  *		x - expression that should be true or error is logged
  *		s - error string to be logged
  */
#define CHECK(x, s) { \
	if ( !(x) ) { \
		std::string errorString = "System error"; \
		errorString.append(s); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

 /*
  * CHECK(): check for general error and log
  *
  * @params:
  *		x - expression that should be true or error is logged
  *		s - error string to be logged
  */
#define CHECK_NOTHROW(x, s) { \
	if ( !(x) ) { \
		std::string errorString = "System error"; \
		errorString.append(s); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};

/*
 * CHECK_SDL_NEGATIVE_ERROR(): checks error for SDL functions returning zero or negative numbers, throws exception
 *
 * @params:
 *		x - result value that should be zero on success, less than zero on error
 */
#define CHECK_SDL_NEGATIVE_ERROR(x) { \
	if ( x<0 ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error"; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

 /*
  * CHECK_SDL_NEGATIVE_ERROR_NOTHROW(): checks error for SDL functions returning zero or negative numbers
  *
  * @params:
  *		x - result value that should be zero on success, less than zero on error
  */
#define CHECK_SDL_NEGATIVE_ERROR_NOTHROW(x) { \
	if ( x<0 ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error"; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};

/*
 * CHECK_SDL_FALSE_ERROR(): checks error for SDL functions returning nullptr on error, throws exception
 *
 * @params:
 *		x - result value that should be true on success, false on error
 */
#define CHECK_SDL_FALSE_ERROR(x) { \
	if ( !(x) ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error"; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

 /*
  * CHECK_SDL_FALSE_ERROR_NOTHROW(): checks error for SDL functions returning nullptr on error
  *
  * @params:
  *		x - result value that should be true on success, false on error
  */
#define CHECK_SDL_FALSE_ERROR_NOTHROW(x) { \
	if ( !(x) ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error"; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};
