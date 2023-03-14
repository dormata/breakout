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
	g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__)); \
};

 /*
  * CHECK(): check for general error and log
  *
  * @params:
  *		x - expression that should be true or error is logged
  *		s - error string to be logged
  */
#define CHECK(x, s) { \
	if ( !(x) ) { \
		std::string errorString = "System error: "; \
		errorString.append(s); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__)); \
	} \
};

/*
 * CHECK_SDL_NEGATIVE_ERROR():
 *
 * @params:
 *		x - result value that should be zero on success, less than zero on error
 */
#define CHECK_SDL_NEGATIVE_ERROR(x) { \
	if ( x<0 ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__)); \
	} \
};

/*
 * CHECK_SDL_FALSE_ERROR():
 *
 * @params:
 *		x - result value that should be true on success, false on error
 */
#define CHECK_SDL_FALSE_ERROR(x) { \
	if ( !(x) ) { \
		const char* description; \
		description = SDL_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__)); \
	} \
};
