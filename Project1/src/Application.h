/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: 
 */
#pragma once

//***************************
// C++ Includes
//***************************

//***************************
// 3rd Party Includes
//***************************
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"

//***************************
// App Includes
//***************************

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

		static constexpr int WINDOW_WIDTH = 600;
		static constexpr int WINDOW_HEIGHT = 400;

		bool			m_run = true;
		bool			m_fullscreen = false;
		int				m_frameCount = 0;
		int				m_timerFPS = 0;
		int				m_lastFrame = 0;
		int				m_lastTime = 0;
		int				m_fps = 0;
		SDL_Window*		m_window = nullptr;
		SDL_Renderer*	m_renderer = nullptr;
		SDL_Surface*	m_screenSurface = nullptr;
		TTF_Font*		m_font = nullptr;
};
