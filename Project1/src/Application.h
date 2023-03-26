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
#include "SDL2_mixer/SDL_mixer.h"

//***************************
// App Includes
//***************************
#include "Level/Level.h"
#include "TexturePool/TexturePool.h"

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
		void render();
		void delay();

		bool			m_run = true;				// main loop condition
		bool			m_exitGame = false;			// go to end screen
		bool			m_fullscreen = false;		// toggle full screen of game window
		bool			m_isWin = true;				// did game finish on a win
		int				m_frameCount = 0;			// current number of frames rendered within a second
		int				m_timeMS = 0;				// current time (from init) in milliseconds
		int				m_lastTimeMS = 0;			// time of last second 
		int				m_fps = 0;					// number of frames per second
		SDL_Window*		m_window = nullptr;			// window handle
		SDL_Renderer*	m_renderer = nullptr;		// renderer handle
		TTF_Font*		m_font = nullptr;			// font handle
		TTF_Font*		m_fontLives = nullptr;		// font handle
		TTF_Font*		m_fontSmall = nullptr;		// font handle
		uint32_t		m_numLevelCurrent = 0;		// currently active level, iterator for level object vector 

		// Vector with objects from class level
		std::vector<std::shared_ptr<Level>> m_levelObjects;

		// Desired fps
		static constexpr int APP_FPS = 60;

		// Resources paths
		// Fonts
		static constexpr char FONTS_FOLDER_NAME[] = "fonts/";
		static constexpr char FONT1_FOLDER_AND_FILE_NAME[] = "handwriting/handwriting.ttf";
		static constexpr char FONT2_FOLDER_AND_FILE_NAME[] = "hearts/hearts.ttf";
		static constexpr char FONT3_FOLDER_AND_FILE_NAME[] = "sleepy/Asleepy.ttf";

		// Game info
		static constexpr int START_NUM_LIVES = 6;
		static constexpr int FONT_SIZE = 30;
		static constexpr int SMALL_FONT_SIZE = 15;
		static constexpr int LIVES_FONT_SIZE = 50;

		struct HUDInfo
		{
			std::string levelName{};
			int			currentScore = 0;;
			int			livesLeft = 0;;
			int			fps = 0;;
		};
		HUDInfo m_hudInfo{};

		// Write text
		void writeText(std::string text, Point location, TTF_Font* font);

		// Background rect
		SDL_Rect m_backgroundRect{};

		// Textures
		std::unique_ptr<TexturePool> m_levelTexPool;
};
