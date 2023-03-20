/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Shared data structures used by File Reader module
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <string>

 //***************************
 // 3rd Party Includes
 //***************************

 //***************************
 // App Includes
 //***************************

//***************************
// Definition
//***************************

struct FileReaderOutputData
{
	std::string		levelName;
	std::string		configFilePathAndName;
};