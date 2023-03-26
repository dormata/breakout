/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Texture Pool header
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
#include "SDL2_image/SDL_image.h"

 //***************************
 // App Includes
 //***************************

 //***************************
 // Definition
 //***************************

class TexturePool
{
public:
	TexturePool();
	~TexturePool();

	uint32_t addTextureToPool(SDL_Texture* texture);
	SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, std::string filename);

	SDL_Texture* getTextureFromVector(uint32_t index) const;

private:
	std::vector<SDL_Texture*> m_texturePointerVector{};
};
