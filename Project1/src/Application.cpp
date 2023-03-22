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
#include "common/DataStructures/FileReaderDataStructures.h"
#include "FileReader/FileReader.h"

//***************************
// Implementation
//***************************

/*
 * initialize(): init objects, 3rd party libs
 */
void Application::initialize()
{
	// Init and set SDL
	CHECK_SDL_NEGATIVE_ERROR(SDL_Init(SDL_INIT_EVERYTHING));

	m_window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	CHECK_SDL_FALSE_ERROR(m_window);

	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_ShowCursor(1)); // Show cursor
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); // TODO: maybe different

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	CHECK_SDL_FALSE_ERROR(m_renderer);

	TTF_Init();
	m_font = TTF_OpenFont("font.ttf", 10);

	// Create File Reader
	std::unique_ptr<FileReader> fileReader = std::make_unique<FileReader>();

	// Create level objects
	std::vector<FileReaderOutputData> configFileNames = fileReader->getConfigFileNames();
	for (uint32_t i = 0; i < configFileNames.size(); i++)
	{
		m_levelObjects.push_back(Level::makeLevel(configFileNames.at(i)));
	}
}

/*
 * execute(): execute main loop
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
		// Active level
		// level->isLevelFinished // ovisno o broju ziovta ili broju brickova
		m_numLevelCurrent = 0; // jel bi ovo sve mozda islo pod game loop

		

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
 * destroy(): release & destroy
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
 * update(): update environment state - fps, window still open
 */
void Application::update()
{
	if (m_fullscreen) CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN));
	if (!m_fullscreen) CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, 0));

	m_timeMS = SDL_GetTicks();
	if (m_timeMS >= (m_lastTimeMS + 1000)) // 1000 ms - check if one second has passed
	{
		m_lastTimeMS = m_timeMS;
		m_fps = m_frameCount;
		m_frameCount = 0;

		// debug
		std::cout << "m_fps = " << m_fps << std::endl;
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
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255));
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderClear(m_renderer));

	// Background
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255));
	SDL_Rect rect{};
	rect.x = 0; rect.y = 0;
	rect.w = WINDOW_WIDTH;
	rect.h = WINDOW_HEIGHT;
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderFillRect(m_renderer, &rect));
	
	m_levelObjects.at(m_numLevelCurrent)->setRendererHandle(m_renderer);
	m_levelObjects.at(m_numLevelCurrent)->fillRenderLevelBuffer();

	// Present final drawing
	SDL_RenderPresent(m_renderer);
}

/*
 * delay(): calculate duration of frame and delay to keep at constant fps
 */
void Application::delay()
{
	int timerFPS = SDL_GetTicks() - m_timeMS;
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
