/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Object for holding sounds
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
#include "SoundPool.h"
#include "../common/Logger.h"
#include "../common/DataPaths.h"

//***************************
// Implementation
//***************************

/*
 * SoundPool(): initialize
 */
SoundPool::SoundPool()
{

}

/*
 * ~SoundPool(): release and destroy
 */
SoundPool::~SoundPool()
{
	for (uint32_t i = 0; i < m_soundPointerVector.size(); i++)
	{
		if (m_soundPointerVector.at(i))
		{
			Mix_FreeChunk(m_soundPointerVector.at(i));
			m_soundPointerVector.at(i) = nullptr;
		}
	}
}

/*
 * loadSoundFromFile(): load sound from file
 */
Mix_Chunk* SoundPool::loadSoundFromFile(std::string filename)
{
	std::string rsrcPath = RESOURCE_FILES_PATH;
	std::string fullPath = rsrcPath + filename;
	Mix_Chunk* sound = Mix_LoadWAV(fullPath.c_str());
	CHECK_MIXER_FALSE_ERROR(sound);

	return sound;
}

/*
 * addSoundToPool(): add sound reference to holder vector
 *
 * @params:
 *		texture - sound (chunk) reference
 *
 * @return: current sound pushed index
 */
uint32_t SoundPool::addSoundToPool(Mix_Chunk* sound)
{
	m_soundPointerVector.push_back(sound);
	uint32_t size = static_cast<unsigned int>(m_soundPointerVector.size());
	return (size - 1);
}

/*
 * getSoundFromVector(): get sound handle from member vector at index
 *
 * @params:
 *		index - index of sound handle from member vector
 *
 * @return: pointer to sound
 */
Mix_Chunk* SoundPool::getSoundFromVector(uint32_t index) const
{
	if (index >= m_soundPointerVector.size())
	{
		LOG("Sound index out of bounds");
		return nullptr;
	}
	return m_soundPointerVector.at(index);
}