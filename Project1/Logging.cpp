/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
 */

 //***************************
 // C++ Includes
 //***************************
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <time.h>

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "Logging.h"

//***************************
// Implementation
//***************************

/*
 * Logging(): initialize
 */
Logging::Logging()
{

}

/*
 * ~Logging(): release and destroy
 */
Logging::~Logging()
{

}

/*
 * reportLog():
 *
 * @params:
 *		expr - string to be logged
 *		fileName - file name from which log is received
 *		line - line in fileName
 */
void Logging::reportLog(std::string expr, std::string fileName, std::string line)
{
	std::string logTimestamp = getDateTimeString();
	std::cout << logTimestamp << " " << expr << " in: " << fileName << " at: " << line << std::endl;

	throw std::runtime_error("FATAL ERROR, SHUTDOWN.");
}

/*
 * getDateTimeString():
 * 
 * @return: returns timestamp
 */
std::string Logging::getDateTimeString()
{
	std::time_t t = std::time(nullptr);
	std::tm localTime;
	gmtime_s(&localTime, &t);
	std::ostringstream timeString;
	timeString << std::put_time(&localTime, "%Y-%m-%d-%H-%M-%S");
	std::string localTimeStr = timeString.str();

	return ("[" + localTimeStr + "]");
}