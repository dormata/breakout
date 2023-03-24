/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
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
#include "Paddle.h"
#include "../../../common/Logger.h"

//***************************
// Implementation
//***************************

/*
 * Paddle(): initialize
 */
Paddle::Paddle()
{

}

/*
 * ~Paddle(): release and destroy
 */
Paddle::~Paddle()
{

}

/*
 * movePaddle(): update location of paddle wrt key inputs
 *
 * @params:
 *		keystates - input keys states
 */
void Paddle::movePaddle(const uint8_t* keystates)
{
	if (keystates[SDL_SCANCODE_LEFT])
	{
		if ((m_properties.x - m_speed) > 0)
		{
			m_properties.x -= m_speed;
		}
	}
	if (keystates[SDL_SCANCODE_RIGHT])
	{
		if ((m_properties.x + m_properties.w + m_speed) < WINDOW_WIDTH)
		{
			m_properties.x += m_speed;
		}
	}
}

/*
 * fillRenderBuffer(): fill buffer with data to be rendered
 */
void Paddle::fillRenderBuffer()
{
	// Set texture/color
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderHandle, 255, 0, 0, 255));
	// Fill buffer with one brick
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderFillRect(m_renderHandle, &m_properties));
}