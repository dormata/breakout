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
#include "IDynamicObject.h"
#include "../../common/Logger.h"

//***************************
// Implementation
//***************************

/*
 * setSpeed(): set object speed - number of pixels changed in one step
 *
 * @params:
 *		speed - object speed
 */
void IDynamicObject::setSpeed(int speed)
{
	m_speed = speed;
}

/*
 * setSize():
 *
 * @params:
 *		w - paddle width
 *		h - paddle height
 */
void IDynamicObject::setSize(int w, int h)
{
	m_properties.w = w;
	m_properties.h = h;
}

/*
 * setInitialPosition(): set from where object starts
 *
 * @params:
 *		x - x value
 *		y - y value
 */
void IDynamicObject::setInitialPosition(int x, int y)
{
	m_properties.x = x;
	m_properties.y = y;
}

/*
 * setRendererHandle(): set renderer handle from Level
 *
 * @params:
 *		renderHandle - renderer handle (pointer)
 */
void IDynamicObject::setRendererHandle(SDL_Renderer* renderHandle)
{
	m_renderHandle = renderHandle;
}

/*
 * getObjectProps(): get object properties
 *
 * @return: object properties
 */
SDL_Rect IDynamicObject::getObjectProps()
{
	return m_properties;
}