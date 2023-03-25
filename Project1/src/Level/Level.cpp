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
	// Create and set Paddle object
	m_objectPaddle = std::make_unique<Paddle>();
	setPaddleData();

	// Create and set Ball object
	m_objectBall = std::make_unique<Ball>();
	setBallData();
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
 * updateGameState():
 */
void Level::updateGameState()
{
	BallVelocity	ballVelocity{}; // x, y components
	SDL_Rect		ballProps{};
	SDL_Rect		paddleProps{};
	int				ballSpeed{}; // number of pixels moved per iteration

	// Get properties
	paddleProps		= m_objectPaddle->getObjectProps();
	ballProps		= m_objectBall->getObjectProps();
	ballVelocity	= m_objectBall->getVelocity();
	ballSpeed		= m_objectBall->getSpeed();

	// Ball vs paddle
	if (hasIntersection(ballProps, paddleProps))
	{
		// Part of paddle hit -> different angle change
		float paddlePart = (paddleProps.x + static_cast<float>(paddleProps.w/2)) - (ballProps.x + static_cast<float>(ballProps.w/2));
		// Normalize - the closer to the middle, the smaller the coeff
		// Keep the sign
		float norm = paddlePart / (paddleProps.w / 2);
		// Hit on the edge, norm > 1
		if (norm > 1)	norm = 1;
		if (norm < -1)	norm = -1;

		float bounceAngle = norm * (5 * 3.14f / 12); // using a bit less than 6pi/12
		ballVelocity.Y = static_cast<int>(std::round(-ballSpeed * std::cos(bounceAngle)));
		ballVelocity.X = static_cast<int>(std::round(ballSpeed * (-std::sin(bounceAngle))));
	}
		
	// Ball vs brick
	SDL_Rect brickProps{};
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		// Get hit points required
		// Get hit points left
		// set opacity or remove
		//m_brickObjects.at(i)->get

		if (m_brickObjects.at(i)->getBrickExists())
		{
			brickProps = m_brickObjects.at(i)->getBrickProps();
			if (hasIntersection(ballProps, brickProps))
			{
				// Added speed tolerances
				
				// Left side hit
				if ((ballProps.x + ballVelocity.X + ballProps.w) >= brickProps.x &&
					(ballProps.x - ballVelocity.X + ballProps.w) <= brickProps.x)
				{
					ballVelocity.X = -ballVelocity.X;
				}
				// Right side hit
				if ((ballProps.x + ballVelocity.X) >= (brickProps.x + brickProps.w) &&
					(ballProps.x - ballVelocity.X) <= (brickProps.x + brickProps.w))
				{
					ballVelocity.X = -ballVelocity.X;
				}

				// Top side hit
				if ((ballProps.y + ballVelocity.Y + ballProps.h) >= brickProps.y &&
					(ballProps.y - ballVelocity.Y + ballProps.h) <= brickProps.y)
				{
					ballVelocity.Y = -ballVelocity.Y;
				}
				// Bottom side hit
				if ((ballProps.y + ballVelocity.Y) <= (brickProps.y + brickProps.h) &&
					(ballProps.y - ballVelocity.Y) >= (brickProps.y + brickProps.h))
				{
					ballVelocity.Y = -ballVelocity.Y;
				}
			}
		}
		
	}

	// Set bricks

	// Ball vs wall
	if (ballProps.x < 0 || (ballProps.x + ballProps.w) > WINDOW_WIDTH) ballVelocity.X = -ballVelocity.X;

	// Ball vs ceiling
	if (ballProps.y <= 0) ballVelocity.Y = -ballVelocity.Y;

	// Check for velocity component limits
	/*if (ballVelocity.X > MAX_X_VAL) ballVelocity.X = MAX_X_VAL;*/

	// Set velocity
	m_objectBall->setVelocity(ballVelocity);

	// Update ball position
	m_objectBall->updatePosition();
}

/*
 * fillRenderLevelBuffer(): fill buffer with data to be rendered
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

	// Bricks
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		m_brickObjects.at(i)->fillRenderBricksBuffer();
	}

	// Paddle
	m_objectPaddle->fillRenderBuffer();

	// Ball
	m_objectBall->fillRenderBuffer();
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

		// When row is finished, increment row counter
		if ((i % m_mainAttributesStruct.colCount) == 0) j++;

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
 * setPaddleData(): set data needed by Paddle object
 */
void Level::setPaddleData()
{
	int paddleWidthPixels = 50;
	int paddleHeightPixels = 10;
	m_objectPaddle->setSize(paddleWidthPixels, paddleHeightPixels);

	int paddleSpeed = 10;
	m_objectPaddle->setSpeed(paddleSpeed);

	int xInital = WINDOW_WIDTH / 2 - paddleWidthPixels/2;
	int yInitial = WINDOW_HEIGHT - (LOWER_SCREEN_OFFSET_PIXELS + paddleHeightPixels);
	m_objectPaddle->setInitialPosition(xInital, yInitial);
}

/*
 * setBallData(): set data needed by Ball object
 */
void Level::setBallData()
{
	int ballWidthPixels = 8;
	int ballHeightPixels = 8;
	m_objectBall->setSize(ballWidthPixels, ballHeightPixels);

	int ballSpeed = 4;
	m_objectBall->setSpeed(ballSpeed);

	// TODO: check where lowest row of bricks is, not window height/2
	int xInital = WINDOW_WIDTH / 2 - ballWidthPixels / 2;
	int yInitial = WINDOW_HEIGHT - (LOWER_SCREEN_OFFSET_PIXELS + WINDOW_HEIGHT/2) - 100;
	m_objectBall->setInitialPosition(xInital, yInitial);

	BallVelocity initVel{};
	initVel.X = 0;
	initVel.Y = ballSpeed;
	m_objectBall->setVelocity(initVel);
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

	// Set for Paddle
	m_objectPaddle->setRendererHandle(m_renderHandle);

	// Set for Ball
	m_objectBall->setRendererHandle(m_renderHandle);
}

/*
 * setKeystates(): set key states
 *
 * @params:
 *		keystates - input keys states
 */
void Level::setKeystates(const uint8_t* keystates)
{
	// Set for Level
	m_keystates = keystates;

	// Set for Paddle
	m_objectPaddle->movePaddle(keystates);
}

/*
 * hasIntersection(): check if rectangles have intersection (collision detection)
 *
 * @params:
 *		rect1 - first rectangle
 *		rect2 - second rectangle
 *
 * @return: true if 2 rectangles have intersection, false otherwise
 */
bool Level::hasIntersection(SDL_Rect rect1, SDL_Rect rect2)
{
	// ---------> x
	// |				l2
	// |				x-----------x
	// y	l1			|			|
	//		x-----------|----x		|
	//		|			|	 |		|
	//		|			x----|------x r2
	//		|				 |
	//		x----------------x
	//						 r1

	// l1 - top left first rect
	int l1X = rect1.x;
	int l1Y = rect1.y;
	// r1 - bottom right first rect
	int r1X = rect1.x + rect1.w;
	int r1Y = rect1.y + rect1.h;

	// l2 - top left second rect
	int l2X = rect2.x;
	int l2Y = rect2.y;
	// r2 - bottom right second rect
	int r2X = rect2.x + rect2.w;
	int r2Y = rect2.y + rect2.h;

	// TODO: if rectangle has area 0

	// If one rectangle is on left side of other
	if (l1X > r2X || l2X > r1X)
	{
		return false;
	}

	// If one rectangle is above other
	if (r1Y < l2Y || r2Y < l1Y)
	{
		return false;
	}

	return true;
}

///*
// * hasIntersection(): check if rectangles have intersection (collision detection)
// *					: specific to ball and paddle - collision from above
// *
// * @params:
// *		rect1 - first rectangle
// *		rect2 - second rectangle
// * 
// * @return: true if 2 rectangles have intersection
// */
//bool Level::hasIntersection(SDL_Rect rect1, SDL_Rect rect2)
//{
//	//			  w1
//	//			x----x
//	//			|	 | h1
//	//			x----x					  w1
//	//	x--------------------x			x----x x--------------------x
//	//	|					 | h2		|	 | |					| h2
//	//	x--------------------x			x----x x--------------------x
//	//			  w2									 w2
//
//	if ((rect1.x + rect1.w) >= rect2.x &&	// from the left side
//		(rect1.x + rect1.w) <= (rect2.x + rect2.w) &&
//		(rect1.y + rect1.h) >= rect2.y ||
//		 rect1.x >= (rect2.x + rect2.w) &&
//		 rect1.x <= rect2.y &&
//		(rect1.y + rect1.h) >= rect2.y)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}