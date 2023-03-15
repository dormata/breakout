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
#include "SDL.h"
#include "SDL_ttf.h"

//***************************
// App Includes
//***************************

//***************************
// Definition
//***************************

class Application
{
	public:
		static Application* getInstance();
		int execute();

	private:
		static Application App;
		Application();
		void initialize();
		void destroy();

		void onEvent(SDL_Event* event);
		void update();
		void gameLoop();
		void render();

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
		TTF_Font*		m_font = nullptr;
};
