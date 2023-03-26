/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: 
 */

 //***************************
 // C++ Includes
 //***************************

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "Brick.h"

//***************************
// Implementation
//***************************

/*
 * Level(): initialize
 */
Brick::Brick()
{

}

/*
 * ~Level(): release and destroy
 */
Brick::~Brick()
{

}

/*
 * onHit(): update brick state when hit
 */
void Brick::onHit()
{
	m_hitsLeftToBreak -= 1;
	if (m_brickAttributes.isBreakable == true)
	{
		if (m_hitsLeftToBreak <= 0)
		{
			// Brick broken
			m_doesBrickExist = false;
			// (zero for hit, predefined score for break)
			m_scoreOnHit = m_brickAttributes.breakScore;
			// Play break sound
			playSound(m_brickAttributes.breakSound);
		}
		else
		{
			// Brick still exists, only hit
			// Play hit sound
			playSound(m_brickAttributes.hitSound);
		}
	}
	else
	{
		// Impenetrable brick
		m_hitsLeftToBreak = 0;
		m_scoreOnHit = 0;
		// Play hit sound
		playSound(m_brickAttributes.hitSound);
	}

	// TODO:
	// set opacity, or cracks, or count to show brick has been hit/damaged but not broken
}

/*
 * playSound(): play sound
 * 
 * @params:
 *		sound - sound to be played
 */
void Brick::playSound(Mix_Chunk* sound)
{
	CHECK_MIXER_NEGATIVE_ERROR_NOTHROW(Mix_PlayChannel(-1, sound, 0));
}

/*
 * makeBrick(): factory method for creating multiple objects of class Brick
 * 
 * @params:
 *		brickAtt - structure required to initialize object brick
 *		brickProperties - size and location of brick
 * 
 * @return: pointer to brick object
 */
std::shared_ptr<Brick> Brick::makeBrick(BrickAttributes brickAtt, SDL_Rect brickProperties)
{
	std::shared_ptr<Brick> objectBrick = std::make_shared<Brick>();

	objectBrick->setBrickExists(true);
	objectBrick->setBrickAtts(brickAtt);
	objectBrick->setBrickRect(brickProperties);
	objectBrick->setHitsLeft(brickAtt.hitPoints);

	return objectBrick;
}

/*
 * fillRenderBricksBuffer(): fill buffer with data to be rendered
 */
void Brick::fillRenderBricksBuffer()
{
	if (m_doesBrickExist)
	{
		// Set texture, now color
		CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderHandle, 255, 0, 0, 255));
		// Fill buffer with one brick
		CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderFillRect(m_renderHandle, &m_brickRect));
	}
}

/*
 * setHitsLeft(): set how many hits are left until brick breaks
 *
 * @params:
 *		hitsLeft - hits left to break brick
 */
void Brick::setHitsLeft(int hitsLeft)
{
	m_hitsLeftToBreak = hitsLeft;
}

/*
 * setBrickExists(): set flag for brick visibility
 *
 * @params:
 *		brickExists - true if bricks should be visible
 */
void Brick::setBrickExists(bool brickExists)
{
	m_doesBrickExist = brickExists;
}

/*
 * setBrickAtts(): set brick attributes
 * 
 * @params:
 *		brickAtts - brick attributes
 */
void Brick::setBrickAtts(BrickAttributes brickAtts)
{
	m_brickAttributes = brickAtts;
}

/*
 * setBrickRect(): set brick rectangle
 *
 * @params:
 *		brickRect - brick rectangle
 */
void Brick::setBrickRect(SDL_Rect brickRect)
{
	m_brickRect = brickRect;
}

/*
 * setRendererHandle(): set renderer handle from Level
 *
 * @params:
 *		renderHandle - renderer handle (pointer)
 */
void Brick::setRendererHandle(SDL_Renderer* renderHandle)
{
	m_renderHandle = renderHandle;
}

/*
 * setHitSoundHandle(): set hit sound handle from Level
 *
 * @params:
 *		sound - sound handle (pointer)
 */
void Brick::setHitSoundHandle(Mix_Chunk* sound)
{
	m_brickAttributes.hitSound = sound;
}

/*
 * setHitSoundHandle(): set break sound handle from Level
 *
 * @params:
 *		sound - sound handle (pointer)
 */
void Brick::setBreakSoundHandle(Mix_Chunk* sound)
{
	m_brickAttributes.breakSound = sound;
}

/*
 * setTextureIndex(): set index for texture vector
 *
 * @params:
 *		index - texture vector index
 */
void Brick::setTextureIndex(uint32_t index)
{
	m_brickAttributes.textureVectorIndex = index;
}

/*
 * setHitSoundIndex(): set index for hit sound vector
 *
 * @params:
 *		index - sound vector index
 */
void Brick::setHitSoundIndex(uint32_t index)
{
	m_brickAttributes.hitSoundVectorIndex = index;
}

/*
 * setBreakSoundIndex(): set index for break sound vector
 *
 * @params:
 *		index - sound vector index
 */
void Brick::setBreakSoundIndex(uint32_t index)
{
	m_brickAttributes.breakSoundVectorIndex = index;
}

/*
 * getBrickProps(): get brick location and size
 *
 * @return: brick properties
 */
SDL_Rect Brick::getBrickProps() const
{
	return m_brickRect;
}

/*
 * getBrickExists(): get does brick exist bool
 *
 * @return: true if brick exists
 */
bool Brick::getBrickExists() const
{
	return m_doesBrickExist;
}

/*
 * getBrickBreakScore(): get how many points current hit is worth 
 *
 * @return: score on break
 */
int Brick::getBrickScore() const
{
	return m_scoreOnHit;
}

/*
 * getBrickBreakable(): get brick breakable status
 *
 * @return: true if brick is breakable
 */
bool Brick::getBrickBreakable() const
{
	return m_brickAttributes.isBreakable;
}

/*
 * getTexturePath(): get texture path
 *
 * @return: texture path
 */
std::string Brick::getTexturePath() const
{
	return m_brickAttributes.texturePath;
}

/*
 * getTextureIndex(): get index of texture in texture vector
 *
 * @return: texture index
 */
uint32_t Brick::getTextureIndex() const
{
	return m_brickAttributes.textureVectorIndex;
}

/*
 * getHitSoundIndex(): get index of hit sound in sound vector
 *
 * @return: sound index
 */
uint32_t Brick::getHitSoundIndex() const
{
	return m_brickAttributes.hitSoundVectorIndex;
}

/*
 * getBreakSoundIndex(): get index of break sound in sound vector
 *
 * @return: sound index
 */
uint32_t Brick::getBreakSoundIndex() const
{
	return m_brickAttributes.breakSoundVectorIndex;
}

/*
 * getBrickType(): get type of brick (map key)
 *
 * @return: brick type
 */
char Brick::getBrickType() const
{
	return m_brickAttributes.key;
}