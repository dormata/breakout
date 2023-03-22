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

	return objectBrick;
}

/*
 * fillRenderBricksBuffer():
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