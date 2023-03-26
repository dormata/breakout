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
#include "Ball.h"
#include "../../../common/Logger.h"

//***************************
// Implementation
//***************************

/*
 * Ball(): initialize
 */
Ball::Ball()
{

}

/*
 * ~Ball(): release and destroy
 */
Ball::~Ball()
{

}

/*
 * updatePosition(): move ball wrt to set velocity
 */
void Ball::updatePosition()
{
	m_properties.x += m_ballVelocity.X;
	m_properties.y += m_ballVelocity.Y;
}

/*
 * fillRenderBuffer(): fill buffer with data to be rendered
 */
void Ball::fillRenderBuffer()
{
	// Set texture/color
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderHandle, 0, 0, 255, 255));
	// Fill buffer with one brick
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderFillRect(m_renderHandle, &m_properties));
}

/*
 * setInitialVelocity(): sets velocity (direction)
 * 
 * @params:
 *		ballVelocity - inital velocity to be set
 */
void Ball::setVelocity(BallVelocity ballVelocity)
{
	m_ballVelocity = ballVelocity;
}

/*
 * getVelocity(): gets velocity (direction)
 *
 * @return: ball velocity
 */
BallVelocity Ball::getVelocity() const
{
	return m_ballVelocity;
}