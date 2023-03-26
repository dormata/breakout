/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Data structures shared within Level folder (specific to level)
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

struct BrickAttributes 
{
	std::string texturePath;
	int			hitPoints;
	std::string hitSoundPath;
	std::string breakSoundPath;
	int			breakScore;
	bool		isBreakable;
	uint32_t	textureVectorIndex;
	char		key;
};
