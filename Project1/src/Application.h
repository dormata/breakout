/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Main app header
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
#include "SDL2_ttf/SDL_ttf.h"

//***************************
// App Includes
//***************************
#include "Level/Level.h"

//***************************
// Definition
//***************************

class Application
{
	public:
		static Application& getInstance();
		int execute();

		// do not allow copies
		Application(Application const&) = delete;
		void operator=(Application const&) = delete;

	private:
		Application() = default; // private constructor -> prevent creating new objects
		void initialize();
		void destroy();

		void onEvent(SDL_Event* event);
		void update();
		void gameLoop();
		void render();
		void delay();

		bool			m_run = true;				// main loop condition
		bool			m_fullscreen = false;		// toggle full screen of game window
		int				m_frameCount = 0;			// current number of frames rendered within a second
		int				m_timeMS = 0;				// current time (from init) in milliseconds
		int				m_lastTimeMS = 0;			// time of last second 
		int				m_fps = 0;					// number of frames per second
		SDL_Window*		m_window = nullptr;			// window handle
		SDL_Renderer*	m_renderer = nullptr;		// renderer handle
		SDL_Surface*	m_screenSurface = nullptr;	// surface handle
		TTF_Font*		m_font = nullptr;			// font handle
		uint32_t		m_numLevelCurrent = 0;		// currently active level, iterator for level object vector 

		// Vector with objects from class level
		std::vector<std::shared_ptr<Level>> m_levelObjects;

};
