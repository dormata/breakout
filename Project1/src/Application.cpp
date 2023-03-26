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
#include "common/DataStructures/CommonDataStructures.h"
#include "FileReader/FileReader.h"
#include "common/DataPaths.h"

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

	m_window = SDL_CreateWindow("Save your cow friends!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	CHECK_SDL_FALSE_ERROR(m_window);

	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_ShowCursor(1)); // Show cursor
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); // TODO: maybe different

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	CHECK_SDL_FALSE_ERROR(m_renderer);

	// Font
	CHECK_TTF_NEGATIVE_ERROR(TTF_Init());
	// Big text
	std::string fontPathAndName = std::string(RESOURCE_FILES_PATH) + std::string(FONTS_FOLDER_NAME) + std::string(FONT1_FOLDER_AND_FILE_NAME);
	m_font = TTF_OpenFont(fontPathAndName.c_str(), FONT_SIZE);
	CHECK_TTF_FALSE_ERROR(m_font);
	// Lives count specific
	fontPathAndName = std::string(RESOURCE_FILES_PATH) + std::string(FONTS_FOLDER_NAME) + std::string(FONT2_FOLDER_AND_FILE_NAME);
	m_fontLives = TTF_OpenFont(fontPathAndName.c_str(), LIVES_FONT_SIZE);
	CHECK_TTF_FALSE_ERROR(m_fontLives);
	// Very small text
	fontPathAndName = std::string(RESOURCE_FILES_PATH) + std::string(FONTS_FOLDER_NAME) + std::string(FONT3_FOLDER_AND_FILE_NAME);
	m_fontSmall = TTF_OpenFont(fontPathAndName.c_str(), SMALL_FONT_SIZE);
	CHECK_TTF_FALSE_ERROR(m_fontSmall);

	// Sound mixer
	CHECK_MIXER_NEGATIVE_ERROR(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048));

	// Create File Reader
	std::unique_ptr<FileReader> fileReader = std::make_unique<FileReader>();

	// Create level objects
	std::vector<FileReaderOutputData> configFileNames = fileReader->getConfigFileNames();
	for (uint32_t i = 0; i < configFileNames.size(); i++)
	{
		m_levelObjects.push_back(Level::makeLevel(configFileNames.at(i)));
	}

	// Textures
	m_backgroundRect.x = 0;
	m_backgroundRect.y = 0;
	m_backgroundRect.w = WINDOW_WIDTH;
	m_backgroundRect.h = WINDOW_HEIGHT;

	// Create Level (backgrounds) texture pool
	m_levelTexPool = std::make_unique<TexturePool>();
	SDL_Texture* addTexture;
	for (uint32_t i = 0; i < m_levelObjects.size(); i++)
	{
		addTexture = m_levelTexPool->loadTextureFromFile(m_renderer, m_levelObjects.at(i)->getBackgroundPath());
		m_levelTexPool->addTextureToPool(addTexture);

		// Create texture pool for bricks
		m_levelObjects.at(i)->setRendererHandle(m_renderer);
		m_levelObjects.at(i)->initTexturePool();
		m_levelObjects.at(i)->setBrickTextureIndices();

		// Create sound pool for bricks
		m_levelObjects.at(i)->initSoundPool();
		m_levelObjects.at(i)->setSoundIndicesAndHandle();
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

	// Set number of lives
	m_hudInfo.livesLeft = START_NUM_LIVES;

	// Start with level
	m_numLevelCurrent = 0;

	// End screen condition
	m_exitGame = false;
	m_isWin = true;

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
		render();

		if (!m_exitGame)
		{
			// Check if level is finished
			if (m_levelObjects.at(m_numLevelCurrent)->areAllBricksBroken() == true)
			{
				// If next level exists, increment index
				int numLvls = static_cast<int>(m_levelObjects.size());
				if (static_cast<int>(m_numLevelCurrent) >= (numLvls-1))
				{
					// Game finished
					m_exitGame = true;
				}
				else
				{
					// Go to next level
					m_numLevelCurrent += 1;
				}
			}
			// Check if player has any lives left
			if (m_hudInfo.livesLeft <= 0)
			{
				m_isWin = false;
				m_exitGame = true;
			}
		}

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

	Mix_Quit();
	IMG_Quit();
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

	// Set keystates
	m_levelObjects.at(m_numLevelCurrent)->setKeystates(keystates);
}

/*
 * update(): update environment state - fps, window still open
 *		   : update game state - ball position
 */
void Application::update()
{
	// Update window settings
	if (m_fullscreen)	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN));
	if (!m_fullscreen)	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetWindowFullscreen(m_window, 0));

	if (!m_exitGame)
	{
		// Update game state - object position, bricks
		m_levelObjects.at(m_numLevelCurrent)->updateGameState();

		// Update HUD info
		m_hudInfo.currentScore += m_levelObjects.at(m_numLevelCurrent)->getScoreChange();
		m_hudInfo.livesLeft += m_levelObjects.at(m_numLevelCurrent)->getLivesChange();
		m_hudInfo.levelName = m_levelObjects.at(m_numLevelCurrent)->getLevelName();
		m_hudInfo.fps = m_fps;

		// Reset delta vars
		m_levelObjects.at(m_numLevelCurrent)->setScoreChange(0);
		m_levelObjects.at(m_numLevelCurrent)->setLivesChange(0);
	}

	m_timeMS = SDL_GetTicks();
	if (m_timeMS >= (m_lastTimeMS + 1000)) // 1000 ms - check if one second has passed
	{
		m_lastTimeMS = m_timeMS;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}
}

/*
 * render():
 */
void Application::render()
{
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255));
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderClear(m_renderer));

	// Background
	SDL_Texture* backgroundTexture = m_levelTexPool->getTextureFromVector(m_numLevelCurrent);
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderCopy(m_renderer, backgroundTexture, NULL, &m_backgroundRect));
	
	// Level objects
	m_levelObjects.at(m_numLevelCurrent)->setRendererHandle(m_renderer);
	m_levelObjects.at(m_numLevelCurrent)->fillRenderLevelBuffer();

	// HUD
	Point textLocation{}; 
	textLocation.x = 10; 
	textLocation.y = 2;
	writeText("SCORE: " + std::to_string(m_hudInfo.currentScore), textLocation, m_font);

	Point livesLocation{};
	livesLocation.x = WINDOW_WIDTH / 2 - FONT_SIZE / 2;
	livesLocation.y = 1;
	writeText(std::to_string(m_hudInfo.livesLeft), livesLocation, m_fontLives);

	textLocation.x = WINDOW_WIDTH - 110;
	textLocation.y = 2;
	writeText("LEVEL: " + m_hudInfo.levelName, textLocation, m_font);

	//textLocation.x = WINDOW_WIDTH - 35; // right
	textLocation.x = 2; // left
	textLocation.y = WINDOW_HEIGHT - SMALL_FONT_SIZE - 2;
	writeText("FPS: " + std::to_string(m_hudInfo.fps), textLocation, m_fontSmall);

	// If game has ended, display end screen until user exits
	if (m_exitGame)
	{
		Point loc{};
		loc.x = WINDOW_WIDTH / 2 - FONT_SIZE / 2 - 50;
		loc.y = WINDOW_HEIGHT /2 - FONT_SIZE / 2 - 50;

		if (m_isWin)
		{
			writeText("COWS SAVED!", loc, m_font);
		}
		else
		{
			writeText("GAME OVER", loc, m_font);
		}
	}


	// Present final drawing
	SDL_RenderPresent(m_renderer);
}

/*
 * delay(): calculate duration of frame and delay to keep at constant fps
 */
void Application::delay()
{
	int timerFPS = SDL_GetTicks() - m_timeMS;
	if (timerFPS < (1000 / APP_FPS)) // 1000 ms = 1 sec
	{
		SDL_Delay((1000 / APP_FPS) - timerFPS);
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

/*
 * writeText(): fills render buffer with text
 * 
 * @params:
 *		text - text to be written
 *		location - starting x & y of text
 *		font - font to be used
 */
void Application::writeText(std::string text, Point location, TTF_Font* font)
{
	// TODO: HORRIBLE! BUILD SDL_FontCache

	SDL_Color color{};
	color.r = 255; color.g = 255; color.b = 255;
	const char* t = text.c_str();
	SDL_Surface* surface;
	SDL_Texture* texture;
	surface = TTF_RenderText_Solid(font, t, color);
	CHECK_TTF_FALSE_ERROR_NOTHROW(surface);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	CHECK_TTF_FALSE_ERROR_NOTHROW(texture);

	SDL_Rect txtRect{};
	txtRect.w = surface->w;
	txtRect.h = surface->h;
	txtRect.x = location.x;
	txtRect.y = location.y;
	//txtRect.x = location.x - txtRect.w;
	//txtRect.y = location.y - txtRect.h;

	SDL_FreeSurface(surface);
	CHECK_TTF_NEGATIVE_ERROR_NOTHROW(SDL_RenderCopy(m_renderer, texture, NULL, &txtRect));
	SDL_DestroyTexture(texture);
}
