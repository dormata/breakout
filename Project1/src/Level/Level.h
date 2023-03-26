/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Level header
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <memory>
#include <array>
#include <map>

 //***************************
 // 3rd Party Includes
 //***************************

 //***************************
 // App Includes
 //***************************
#include "../common/DataStructures/CommonDataStructures.h"
#include "LevelDataStructures.h"
#include "Brick/Brick.h"
#include "IDynamicObject/Paddle/Paddle.h"
#include "IDynamicObject/Ball/Ball.h"
#include "../TexturePool/TexturePool.h"
#include "../SoundPool/SoundPool.h"

 //***************************
 // Definition
 //***************************

class Level 
{
	public:
		Level();
		~Level();

		// Factory method
		static std::shared_ptr<Level> makeLevel(FileReaderOutputData configStruct); // static -> can be used before instancing object

		void setRendererHandle(SDL_Renderer* renderHandle);
		void setKeystates(const uint8_t* keystates);
		void fillRenderLevelBuffer();
		void updateGameState();
		void setLivesChange(int lives);
		void setScoreChange(int score);
		void setBrickTextureIndices();
		void setSoundIndicesAndHandle();

		void initTexturePool();
		void initSoundPool();

		int			getLivesChange() const;
		int			getScoreChange() const;
		std::string getLevelName() const;
		std::string	getBackgroundPath() const;

		bool areAllBricksBroken();

	private:
		void parseXML(std::string filename);
		void setLevelName(std::string name);

		std::string		m_levelName;
		SDL_Renderer*	m_renderHandle = nullptr;
		const uint8_t*	m_keystates = nullptr;

		void processAndCreateBrickLayout();

		bool isEmptyStringElement(char element);

		// XML config file structure
		// Main element
		const char* MAIN_ELEMENT = "Level";

		// Main element attributes -> MUST have all
		const char* ROW_COUNT = "RowCount";
		const char* COL_COUNT = "ColumnCount";
		const char* ROW_SPACING = "RowSpacing";
		const char* COL_SPACING = "ColumnSpacing";
		const char* BACKGROUND_TEXTURE_PATH = "BackgroundTexture";

		struct MainElementAttributes {
			int			rowCount;
			int			colCount;
			int			rowSpacing; // spacing between two neighbouring rows (up-down)
			int			colSpacing; // spacing between two neighbouring columns (left-right)
			std::string backgroundPath;
		};
		MainElementAttributes m_mainAttributesStruct{};

		// First child element number 1
		const char* FIRST_CHILD_ELEMENT_1 = "BrickTypes";
		// Second child element number 1
		const char* SECOND_CHILD_ELEMENT_1 = "BrickType";
		// Brick attributes -> does not need to parse all
		const char* BRICK_ID = "Id";
		const char* BRICK_TEXTURE = "Texture";
		const char* BRICK_HIT_PTS = "HitPoints";
		const char* BRICK_HIT_SOUND = "HitSound";
		const char* BRICK_BREAK_SOUND = "BreakSound";
		const char* BRICK_BREAK_SCORE = "BreakScore";

		// Reserved character for infinite number of hits
		const char* INFINITE_1 = "Infinite";
		const char* INFINITE_2 = "infinite";

		// Map key
		char m_id = 0;

		// Reserved character for empty place in brick layout
		const char RESERVED_EMPTY_CHAR = '_';
		
		// Brick ID with its attributes
		std::map<char, BrickAttributes> m_brickTypesMap;

		// First child element number 2
		const char* FIRST_CHILD_ELEMENT_2 = "Bricks";

		// Brick layout
		std::string m_layoutString;
		int			m_totalNumberOfBricks = 0;
		bool		m_allBricksBroken = false;

		// One brick height in pixels
		static constexpr int BRICK_HEIGHT_PIXELS = 30;
		// Do not render on first number of pixels up on screen
		static constexpr int UPPER_SCREEN_OFFSET_PIXELS = 80;
		// Do not render on first number of pixels down on screen
		static constexpr int LOWER_SCREEN_OFFSET_PIXELS = 18;
		// Do not render on first/last number of pixels left/right on screen
		static constexpr int LEFT_RIGHT_SCREEN_OFFSET_PIXELS = 0; // has to be an even number to look nice because of rounding

		// Brick objects vector
		std::vector<std::shared_ptr<Brick>> m_brickObjects;

		// Paddle
		std::unique_ptr<Paddle> m_objectPaddle;
		void setPaddleData();
		Point m_paddleInitialPos{};

		// Ball
		std::unique_ptr<Ball> m_objectBall;
		void setBallData();
		Point m_ballInitialPos{};
		BallVelocity m_initialBallVel{};

		// Intersection checks
		bool hasIntersection(SDL_Rect rect1, SDL_Rect rect2);
		void updateBallDataOnPaddleHit(const SDL_Rect& paddleProps, const SDL_Rect& ballProps,
										int ballSpeed, BallVelocity& ballVelocity);
		void updateBallDataOnBrickHit(const SDL_Rect& brickProps, const SDL_Rect& ballProps, BallVelocity& ballVelocity);

		// HUD info
		int		m_livesNumChanged = 0;
		int		m_scoreChange = 0;

		// Textures
		std::unique_ptr<TexturePool> m_objectTexturePool;

		// Sounds
		std::unique_ptr<SoundPool> m_objectSoundPoolHit;
		std::unique_ptr<SoundPool> m_objectSoundPoolBreak;
};

