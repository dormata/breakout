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
			m_doesBrickExist = false;
			// (zero for hit, predefined score for break)
			m_scoreOnHit = m_brickAttributes.breakScore;
		}
	}
	else
	{
		m_hitsLeftToBreak = 0;
		m_scoreOnHit = 0;
	}

	// play sounds

	// set opacity
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
 * setTextureIndex(): set index for texture vector
 *
 * @params:
 *		renderHandle - texture vector index
 */
void Brick::setTextureIndex(uint32_t index)
{
	m_brickAttributes.textureVectorIndex = index;
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
 * getBrickType(): get type of brick (map key)
 *
 * @return: brick type
 */
char Brick::getBrickType() const
{
	return m_brickAttributes.key;
}