/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <memory>
#include <array>

 //***************************
 // 3rd Party Includes
 //***************************

 //***************************
 // App Includes
 //***************************
#include "../common/DataStructures/FileReaderDataStructures.h"

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

		void printbla();

	private:
		void parseXML(std::string filename);

		// XML config file structure
		// Main element
		const char* MAIN_ELEMENT = "Level";
		const char* ROW_COUNT = "RowCount";
		const char* COL_COUNT = "ColumnCount";
		const char* ROW_SPACING = "RowSpacing";
		const char* COL_SPACING = "ColumnSpacing";
		const char* BACKGROUND_TEXTURE_PATH = "BackgroundTexture";

		struct MainElementAttributes {
			int			rowCount;
			int			colCount;
			int			rowSpacing;
			int			colSpacing;
			const char* backgroundPath;
		};

		// First child element number 1
		const char* FIRST_CHILD_ELEMENT_1 = "BrickTypes";
		// Second child element number 1
		const char* SECOND_CHILD_ELEMENT_1 = "BrickType";
		static constexpr uint32_t NUM_2ND_ELEMENT_1_ATTRIBUTES = 6;
		std::array <const char*, NUM_2ND_ELEMENT_1_ATTRIBUTES> m_secondElement1AttributeNames
		{
			"Id",
			"Texture",
			"HitPoints",
			"HitSound",
			"BreakSound",
			"BreakScore"
		};

		// First child element number 2
		const char* FIRST_CHILD_ELEMENT_2 = "Bricks";

};

