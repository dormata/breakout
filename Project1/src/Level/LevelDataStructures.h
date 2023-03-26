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
#include "SDL2/SDL.h"
#include "SDL2_mixer/SDL_mixer.h"

 //***************************
 // App Includes
 //***************************

 //***************************
 // Definition
 //***************************

struct BrickAttributes 
{
	std::string		texturePath;
	int				hitPoints;
	std::string		hitSoundPath;
	std::string		breakSoundPath;
	int				breakScore;
	bool			isBreakable;
	uint32_t		textureVectorIndex;
	char			key;
	uint32_t		hitSoundVectorIndex;
	uint32_t		breakSoundVectorIndex;
	SDL_Texture*	texture;
	Mix_Chunk*		hitSound;
	Mix_Chunk*		breakSound;
};
