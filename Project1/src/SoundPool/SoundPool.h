/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Sound Pool header
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <vector>

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

class SoundPool
{
	public:
		SoundPool();
		~SoundPool();

		Mix_Chunk* loadSoundFromFile(std::string filename);
		uint32_t addSoundToPool(Mix_Chunk* sound);

		Mix_Chunk* getSoundFromVector(uint32_t index) const;

	private:
		// Intended for sound effects, not music
		std::vector<Mix_Chunk*> m_soundPointerVector{};
};
