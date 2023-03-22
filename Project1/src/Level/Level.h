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
#include "../common/DataStructures/FileReaderDataStructures.h"
#include "LevelDataStructures.h"
#include "Brick/Brick.h"

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

	private:
		void parseXML(std::string filename);
		void setLevelName(std::string name);
		std::string m_levelName;

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

		// Map key
		char m_id;
		
		// Brick ID with its attributes
		std::map<char, BrickAttributes> m_brickTypesMap;

		// First child element number 2
		const char* FIRST_CHILD_ELEMENT_2 = "Bricks";

		// Brick layout
		std::string m_layoutString;

		// One brick height in pixels
		static constexpr int BRICK_HEIGHT_PIXELS = 20;
		// Do not render on first number of pixels up on screen
		static constexpr int UPPER_SCREEN_OFFSET_PIXELS = 10;
		// Do not render on first/last number of pixels left/right on screen
		static constexpr int LEFT_RIGHT_SCREEN_OFFSET_PIXELS = 5;
};

