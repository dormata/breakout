/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Game level module with objects and methods specific to level - not visible in application
 *			  : handles bricks, sounds, score, lives, textures, ...
 */

 //***************************
 // C++ Includes
 //***************************
#include <iostream>
#include <vector>
#include <cmath>

//***************************
// 3rd Party Includes
//***************************
#include "tinyxml2/tinyxml2.h"

//***************************
// App Includes
//***************************
#include "Level.h"
#include "../common/Logger.h"

//***************************
// Implementation
//***************************

/*
 * Level(): initialize
 */
Level::Level()
{

}

/*
 * ~Level(): release and destroy
 */
Level::~Level()
{

}

/*
 * makeLevel(): factory method for creating multiple objects of class Level
 * 
 * @params:
 *		configStruct - structure required to initialize object
 * 
 * @return: pointer to level object
 */
std::shared_ptr<Level> Level::makeLevel(FileReaderOutputData configStruct)
{
	std::shared_ptr<Level> objectLevel = std::make_shared<Level>();

	// Parse config file
	objectLevel->parseXML(configStruct.configFilePathAndName);
	// Level name
	objectLevel->setLevelName(configStruct.levelName);
	// Brick layout
	objectLevel->processAndCreateBrickLayout();

	return objectLevel;
}

/*
 * fillRenderBuffer():
 */
void Level::fillRenderLevelBuffer()
{
	// Background
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_SetRenderDrawColor(m_renderHandle, 0, 255, 0, 255));
	SDL_Rect rect{};
	rect.x = 0; rect.y = 0;
	rect.w = 600;
	rect.h = 100;
	CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderFillRect(m_renderHandle, &rect));

	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		m_brickObjects.at(i)->fillRenderBricksBuffer();
	}
}

/*
 * processAndCreateBrickLayout():
 */
void Level::processAndCreateBrickLayout()
{
	// Assume layout is correctly defined w.r.t. required number of rows and columns
	int totalNumberBricks = m_mainAttributesStruct.rowCount * m_mainAttributesStruct.colCount;

	// _______________________________ <- window width
	//
	// x-----x x-----x x-----x x-----x
	// |     | |     | |     | |     |
	// x-----x x-----x x-----x x-----x
	// x-----x x-----x x-----x x-----x
	// |     | |     | |     | |     |
	// x-----x x-----x x-----x x-----x

	float totalBrickWidthPixels = static_cast<float>(WINDOW_WIDTH) 
											- ((m_mainAttributesStruct.colCount - 1) * m_mainAttributesStruct.colSpacing
											+ 2 * LEFT_RIGHT_SCREEN_OFFSET_PIXELS);
	int brickWidthPixels = static_cast<int>(std::round(totalBrickWidthPixels / m_mainAttributesStruct.colCount));

	SDL_Rect brickProps{};
	brickProps.w = brickWidthPixels;
	brickProps.h = BRICK_HEIGHT_PIXELS;

	BrickAttributes brickAttributes{};
	int lastIndex = 0; int j = -1;
	for (int i = 0; i < totalNumberBricks; i++)
	{
		// Column iterator
		brickProps.x = (i % m_mainAttributesStruct.colCount) * brickProps.w
					 + (i % m_mainAttributesStruct.colCount) * m_mainAttributesStruct.colSpacing
					 + LEFT_RIGHT_SCREEN_OFFSET_PIXELS
					 + m_mainAttributesStruct.colSpacing/2;

		if ((i % m_mainAttributesStruct.colCount) == 0)
		{
			// When row is finished, increment row counter
			j++;
		}

		// Row iterator
		brickProps.y = (j % m_mainAttributesStruct.rowCount) * brickProps.h
					 + (j % m_mainAttributesStruct.rowCount) * m_mainAttributesStruct.rowSpacing
					 + UPPER_SCREEN_OFFSET_PIXELS;
	

		// Find first brick ID in layout
		while (isEmptyStringElement(m_layoutString[lastIndex])) { lastIndex++; }
		// Check brick type
		bool successPair = false;
		for (auto const& [key, val] : m_brickTypesMap)
		{
			if (key == m_layoutString[lastIndex])
			{
				brickAttributes = val;
				successPair = true;

				// Brick paired with key -> create brick
				m_brickObjects.push_back(Brick::makeBrick(brickAttributes, brickProps));

				break;
			}
		}
		if (!successPair)
		{
			if (m_layoutString[lastIndex] == RESERVED_EMPTY_CHAR)
			{
				// empty space, skip init
			}
			else
			{
				// unknown brick key -> layout unknown!
				std::string msg(1, m_layoutString[lastIndex]);
				LOG_AND_THROW("Brick ID: " + msg + " not recognized - not possible to parse layout!");
			}
		}
		lastIndex++;	
	}

}

/*
 * setLevelName(): set level name
 * 
 * @params:
 *		name - name of level
 */
void Level::setLevelName(std::string name)
{
	m_levelName = name;
}

/*
 * parseXML(): parse XML file according to predefined structrure and "sets" config data for level object
 *			 : data parsed to member vars -> this function should be called by object in which these memvars should be set
 * 
 * @params:
 *		filename - name of XML file to parse
 */
void Level::parseXML(std::string filename)
{
	const char* filenameChar = filename.c_str();
	tinyxml2::XMLDocument doc;
	CHECK_TINYXML_ERROR(doc.LoadFile(filenameChar), doc);

	// Generally returns error for a poorly constructed XML file, or file wihout XML
	tinyxml2::XMLNode* pRoot = doc.FirstChild();
	CHECK(pRoot, "XML file read error!");

	// String element name for error logging
	std::string elementNameString;

	// Main element
	tinyxml2::XMLElement* pElement = doc.FirstChildElement(MAIN_ELEMENT);
	elementNameString = MAIN_ELEMENT;
	CHECK(pElement, "XML error parsing element " + elementNameString);
	// Main element attributes
	// 1
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(ROW_COUNT, &m_mainAttributesStruct.rowCount), doc);
	// 2
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(COL_COUNT, &m_mainAttributesStruct.colCount), doc);
	// 3
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(ROW_SPACING, &m_mainAttributesStruct.rowSpacing), doc);
	// 4
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(COL_SPACING, &m_mainAttributesStruct.colSpacing), doc);
	// 5
	const char* backPath;
	CHECK_TINYXML_ERROR(pElement->QueryStringAttribute(BACKGROUND_TEXTURE_PATH, &backPath), doc);
	m_mainAttributesStruct.backgroundPath = backPath;

	// First child of main element
	tinyxml2::XMLElement* pSubElement1 = pElement->FirstChildElement(FIRST_CHILD_ELEMENT_1);
	elementNameString = FIRST_CHILD_ELEMENT_1;
	CHECK(pSubElement1, "XML error parsing element " + elementNameString);

	// Brick type repeating unknown number of times
	BrickAttributes brickAttStruct{};
	for (tinyxml2::XMLElement* pRepeatingElement = pSubElement1->FirstChildElement(SECOND_CHILD_ELEMENT_1);
		pRepeatingElement != nullptr; pRepeatingElement = pRepeatingElement->NextSiblingElement())
	{
		// TODO: add necessity checks - e.g. if hit points == inf then sounds not necessary
		// TODO: add defaults - default value if attribute is not defined in config
		// TODO: add sanity checks - e.g. hit points >100

		// Missing attribute not logged as error here -> attributes do not have to be defined
		// NO macro error checks

		// Id
		const char* idChar;
		if (pRepeatingElement->QueryStringAttribute(BRICK_ID, &idChar) != tinyxml2::XML_SUCCESS)
		{
			LOG_AND_THROW("Brick ID not defined - not possible to parse layout!");
		}
		m_id = idChar[0];

		// Texture path
		const char* textPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_TEXTURE, &textPath) != tinyxml2::XML_SUCCESS)
		{
			// use default texture
		}
		brickAttStruct.texturePath = textPath;

		// Hit points
		if (pRepeatingElement->QueryIntAttribute(BRICK_HIT_PTS, &brickAttStruct.hitPoints) != tinyxml2::XML_SUCCESS)
		{
			// use default num of hit points
		}

		// Brick hit sound path
		const char* hitSndPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_HIT_SOUND, &hitSndPath) != tinyxml2::XML_SUCCESS)
		{
			// use default sound
		}
		brickAttStruct.hitSoundPath = hitSndPath;

		// Break sound path
		const char* brkSndPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_BREAK_SOUND, &brkSndPath) != tinyxml2::XML_SUCCESS)
		{
			// use default sound
		}
		brickAttStruct.breakSoundPath = brkSndPath;

		// Brick break score
		if (pRepeatingElement->QueryIntAttribute(BRICK_BREAK_SCORE, &brickAttStruct.breakScore) != tinyxml2::XML_SUCCESS)
		{
			// use default score
		}

		m_brickTypesMap.insert({ m_id, brickAttStruct });
	}

	// Other first child of main element
	tinyxml2::XMLElement* pSubElement2 = pElement->FirstChildElement(FIRST_CHILD_ELEMENT_2);
	elementNameString = FIRST_CHILD_ELEMENT_2;
	CHECK(pSubElement2, "XML error parsing element " + elementNameString);

	const char* layout = pSubElement2->GetText();
	CHECK(layout, "XML error parsing layout");
	m_layoutString = layout;
}

/*
 * isEmptyStringElement():
 * 
 * @params:
 *		element - element from string
 * 
 * @return: true if element is empty (has empty spaces)
 */
bool Level::isEmptyStringElement(char element)
{
	bool isEmpty = false;
	if ((element == ' ') || (element == '\t') || (element == '\n') || (element == '\r') || (element == '\r\n'))
	{
		isEmpty = true;
	}
	return isEmpty;
}

/*
 * setRendererHandle(): set renderer handle from App in Level, and pass to Brick
 *
 * @params:
 *		renderHandle - renderer handle (pointer)
 */
void Level::setRendererHandle(SDL_Renderer* renderHandle)
{
	// Set for Level
	m_renderHandle = renderHandle;

	// Set for Bricks
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		m_brickObjects.at(i)->setRendererHandle(m_renderHandle);
	}
}