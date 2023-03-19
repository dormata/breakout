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
#include "Application.h"
#include "common/Logger.h"
#include "FileReader/FileReader.h"

//***************************
// Implementation
//***************************

/*
 * initialize():
 */
void Application::initialize()
{
	// Init and set SDL
	CHECK_SDL_NEGATIVE_ERROR(SDL_Init(SDL_INIT_EVERYTHING));

	m_window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	CHECK_SDL_FALSE_ERROR(m_window);

	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_ShowCursor(1)); // show cursor
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); // TODO: ne znam

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	CHECK_SDL_FALSE_ERROR(m_renderer);

	TTF_Init();
	m_font = TTF_OpenFont("font.ttf", 10);

	// Create File Reader
	std::unique_ptr<FileReader> fileReader = std::make_unique<FileReader>();
}

/*
 * execute():
 *
 * @return: 0 if successful, negative otherwise
 */
int Application::execute()
{
	initialize();

	SDL_Event event;
	// Main loop
	while (m_run)
	{
		// Are there pending events
		while (SDL_PollEvent(&event) > 0)
		{
			onEvent(&event);
			if (event.type == SDL_QUIT) m_run = false;
		}

		update();
		gameLoop();
		render();

		m_frameCount++;
		delay();
		
	}

	destroy();
	return 0;
}

/*
 * destroy():
 */
void Application::destroy()
{
	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
	else
	{
		// log error
	}

	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	else
	{
		// log error
	}
	
	SDL_Quit();
}

/*
 * onEvent(): process inputs
 *
 * @params:
 *		event - polled event
 */
void Application::onEvent(SDL_Event* event)
{
	const uint8_t* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDL_SCANCODE_ESCAPE]) m_run = false;
	if (keystates[SDL_SCANCODE_F11]) m_fullscreen = !m_fullscreen;
	//if (keystates[SDL_SCANCODE_LEFT])
}

/*
 * update():
 */
void Application::update()
{
	if (m_fullscreen) CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN));
	if (!m_fullscreen) CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, 0));

	m_lastFrame = SDL_GetTicks();
	if (m_lastFrame >= (m_lastTime + 1000))
	{
		m_lastTime = m_lastFrame;
		m_fps = m_frameCount;
		m_frameCount = 0;

		std::cout << "m_fps = " << m_fps << std::endl; // mozda stavi pod options statistics
	}
}

/*
 * gameLoop():
 */
void Application::gameLoop()
{

}

/*
 * render():
 */
void Application::render()
{
	CHECK_SDL_NEGATIVE_ERROR(SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255));
	//CHECK_SDL_NEGATIVE_ERROR(SDL_RenderClear(m_renderer));

	SDL_Rect rect{};
	rect.x = 0; rect.y = 0;
	rect.w = 300;
	rect.h = 200;
	CHECK_SDL_NEGATIVE_ERROR(SDL_RenderFillRect(m_renderer, &rect));
	
	// Present final drawing
	SDL_RenderPresent(m_renderer);
}

/*
 * delay(): calculate duration of frame and delay to keep at constant fps
 */
void Application::delay()
{
	int timerFPS = SDL_GetTicks() - m_lastFrame;
	if (timerFPS < (1000 / 60))
	{
		SDL_Delay((1000 / 60) - timerFPS);
	}
}

/*
 * getInstance(): create "Application" object on first call and return handle
 *
 * @return: object reference
 */
Application& Application::getInstance()
{ 
	static Application instance;
	return instance;
}
