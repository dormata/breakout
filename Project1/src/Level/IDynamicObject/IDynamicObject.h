/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Dynamic object interface
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************

 //***************************
 // 3rd Party Includes
 //***************************
#include "SDL2/SDL.h"

 //***************************
 // App Includes
 //***************************
#include "../../common/DataStructures/CommonDataStructures.h"

 //***************************
 // Definition
 //***************************

class IDynamicObject
{
	public:
		IDynamicObject() = default;
		~IDynamicObject() = default;

		// Every dynamic object must have these set
		void setInitialPosition(int x, int y);
		void setRendererHandle(SDL_Renderer* renderHandle);

		// Every dynamic object must have these set once or in a loop
		void setSpeed(int speed);
		void setSize(int w, int h);

		// Render
		virtual void fillRenderBuffer() = 0;

		// Get properties
		SDL_Rect getObjectProps() const;
		int		 getSpeed() const;

	protected:
		SDL_Rect		m_properties{};
		int				m_speed = 0;
		SDL_Renderer*	m_renderHandle = nullptr;
};
