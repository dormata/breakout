/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Shared data structures used by any module
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

static constexpr int WINDOW_WIDTH = 600;
static constexpr int WINDOW_HEIGHT = 500;

struct FileReaderOutputData
{
	std::string		levelName;
	std::string		configFilePathAndName;
};

struct BallVelocity
{
	int X = 0;
	int Y = 0;
};

struct Point
{
	int x = 0;
	int y = 0;
};