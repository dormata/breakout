/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Object for holding textures
 */

 //***************************
 // C++ Includes
 //***************************
#include <iostream>

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "TexturePool.h"
#include "../common/Logger.h"
#include "../common/DataPaths.h"

//***************************
// Implementation
//***************************

/*
 * TexturePool(): initialize
 */
TexturePool::TexturePool()
{

}

/*
 * ~TexturePool(): release and destroy
 */
TexturePool::~TexturePool()
{
	for (uint32_t i = 0; i < m_texturePointerVector.size(); i++)
	{
		if (m_texturePointerVector.at(i))
		{
			SDL_DestroyTexture(m_texturePointerVector.at(i));
			m_texturePointerVector.at(i) = nullptr;
		}
	}
}

/*
 * loadTextureFromFile(): create texture from image file
 *
 * @params:
 *		renderer - renderer handle
 *		filename - name and path of file from which to load texture
 * 
 * @return: pointer to created texture
 */
SDL_Texture* TexturePool::loadTextureFromFile(SDL_Renderer* renderer, std::string filename)
{
	std::string rsrcPath = RESOURCE_FILES_PATH;
	std::string fullPath = rsrcPath + filename;
	SDL_Surface* tmpSurface = IMG_Load(fullPath.c_str());
	CHECK_SDL_FALSE_ERROR(tmpSurface);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	CHECK_SDL_FALSE_ERROR(texture);

	//SDL_QueryTexture(texture, NULL, NULL, NULL, NULL);

	SDL_FreeSurface(tmpSurface);

	return texture;
}

/*
 * addTextureToPool(): add texture reference to holder vector
 * 
 * @params:
 *		texture - texture reference
 * 
 * @return: current texture pushed index
 */
uint32_t TexturePool::addTextureToPool(SDL_Texture* texture)
{
	m_texturePointerVector.push_back(texture);
	uint32_t size = static_cast<unsigned int>(m_texturePointerVector.size());
	return (size - 1);
}

/*
 * getTextureFromVector(): get texture handle from member vector at index
 *
 * @params:
 *		index - index of texture from member vector
 *
 * @return: pointer to texture
 */
SDL_Texture* TexturePool::getTextureFromVector(uint32_t index) const
{
	if (index >= m_texturePointerVector.size())
	{
		LOG("Texture index out of bounds");
		return nullptr;
	}
	return m_texturePointerVector.at(index);
}