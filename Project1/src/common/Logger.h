/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Header with error logging macros
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
inline Logging g_errorLogs;

/*
 * LOG(): log error string
 * 
 * @params:
 *		s - error string to be logged
 */
#define LOG(s) { \
	std::string errorString = "System error: "; \
	errorString.append(s); \
	g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
};

 /*
  * LOG_AND_THROW(): log error string and throw exception
  *
  * @params:
  *		s - error string to be logged
  */
#define LOG_AND_THROW(s) { \
	std::string errorString = "System error: "; \
	errorString.append(s); \
	g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
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
		std::string errorString = "System error: "; \
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
		std::string errorString = "System error: "; \
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
		std::string errorString = "System error: "; \
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
		std::string errorString = "System error: "; \
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
		std::string errorString = "System error: "; \
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
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};

 /*
  * CHECK_TINYXML_ERROR(): checks error for tinyxml2 functions, throws exception, TODO: always print error code?
  *
  * @params:
  *		x - result value that should be XML_SUCCESS, false on error
  *		obj - XMLDocument object (public member fcns not static)
  */
#define CHECK_TINYXML_ERROR(x, obj) { \
	tinyxml2::XMLError result = x; \
	if ( result != tinyxml2::XML_SUCCESS ) { \
		const char* description; \
		description = obj.ErrorStr(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

/*
 * CHECK_TTF_NEGATIVE_ERROR(): checks error for TTF functions returning zero or negative numbers, throws exception
 *
 * @params:
 *		x - result value that should be zero on success, less than zero on error
 */
#define CHECK_TTF_NEGATIVE_ERROR(x) { \
	if ( x<0 ) { \
		const char* description; \
		description = TTF_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

/*
* CHECK_SDL_NEGATIVE_ERROR_NOTHROW(): checks error for TTF functions returning zero or negative numbers
*
* @params:
*		x - result value that should be zero on success, less than zero on error
*/
#define CHECK_TTF_NEGATIVE_ERROR_NOTHROW(x) { \
	if ( x<0 ) { \
		const char* description; \
		description = TTF_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};

/*
 * CHECK_TTF_FALSE_ERROR(): checks error for TTF functions returning nullptr on error, throws exception
 *
 * @params:
 *		x - result value that should be true on success, false on error
 */
#define CHECK_TTF_FALSE_ERROR(x) { \
	if ( !(x) ) { \
		const char* description; \
		description = TTF_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 1); \
	} \
};

 /*
  * CHECK_TTF_FALSE_ERROR_NOTHROW(): checks error for TTF functions returning nullptr on error
  *
  * @params:
  *		x - result value that should be true on success, false on error
  */
#define CHECK_TTF_FALSE_ERROR_NOTHROW(x) { \
	if ( !(x) ) { \
		const char* description; \
		description = TTF_GetError(); \
		std::string errorString = "System error: "; \
		errorString.append(description); \
		g_errorLogs.reportLog(errorString, std::string(__FILE__), std::to_string(__LINE__), 0); \
	} \
};
