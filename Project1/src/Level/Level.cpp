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
 * updateGameState(): calculates objects' positions after interactions
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
		updateBallDataOnPaddleHit(paddleProps, ballProps, ballSpeed, ballVelocity);
	}
		
	// Ball vs brick
	SDL_Rect brickProps{};
	m_allBricksBroken = true; // assume true until at least one existing is found
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		if (m_brickObjects.at(i)->getBrickExists())
		{
			if (m_brickObjects.at(i)->getBrickBreakable())
			{
				m_allBricksBroken = false;
			}
			brickProps = m_brickObjects.at(i)->getBrickProps();
			if (hasIntersection(ballProps, brickProps))
			{
				// Update ball props
				updateBallDataOnBrickHit(brickProps, ballProps, ballVelocity);
				// Update brick props
				m_brickObjects.at(i)->onHit();
				// Update score
				m_scoreChange += m_brickObjects.at(i)->getBrickScore();
			}
		}	
	}
	// Ball vs wall
	if (ballProps.x < 0 || (ballProps.x + ballProps.w) > WINDOW_WIDTH) ballVelocity.X = -ballVelocity.X;

	// Ball vs ceiling
	if (ballProps.y <= 0) ballVelocity.Y = -ballVelocity.Y;

	// Set velocity
	m_objectBall->setVelocity(ballVelocity);

	// Ball vs floor
	if (ballProps.y >= WINDOW_HEIGHT)
	{
		m_livesNumChanged = -1;
		// Reset position
		m_objectPaddle->setInitialPosition(m_paddleInitialPos.x, m_paddleInitialPos.y);
		m_objectBall->setInitialPosition(m_ballInitialPos.x, m_ballInitialPos.y);
		m_objectBall->setVelocity(m_initialBallVel);
	}

	// TODO: check for infinite bounces - ball stuck, reset

	// Update ball position
	m_objectBall->updatePosition();
}

/*
 * updateBallDataOnPaddleHit(): change ball velocity x & y components based on where ball hit the current brick
 *
 * @params:
 *		brickProps - brick properties
 *		ballProps - ball properties
 *		ballVelocity - ball velocity = components of speed
 */
void Level::updateBallDataOnBrickHit(const SDL_Rect& brickProps, const SDL_Rect& ballProps, BallVelocity& ballVelocity)
{
	// Added speed tolerances
	// Left brick side hit
	if ((ballProps.x + ballVelocity.X + ballProps.w) >= brickProps.x &&
		(ballProps.x - ballVelocity.X + ballProps.w) <= brickProps.x)
	{
		ballVelocity.X = -ballVelocity.X;
	}
	// Right brick side hit
	if ((ballProps.x + ballVelocity.X) <= (brickProps.x + brickProps.w) &&
		(ballProps.x - ballVelocity.X) >= (brickProps.x + brickProps.w))
	{
		ballVelocity.X = -ballVelocity.X;
	}

	// Top brick side hit
	if ((ballProps.y + ballVelocity.Y + ballProps.h) >= brickProps.y &&
		(ballProps.y - ballVelocity.Y + ballProps.h) <= brickProps.y)
	{
		ballVelocity.Y = -ballVelocity.Y;
	}
	// Bottom brick side hit
	if ((ballProps.y + ballVelocity.Y) <= (brickProps.y + brickProps.h) &&
		(ballProps.y - ballVelocity.Y) >= (brickProps.y + brickProps.h))
	{
		ballVelocity.Y = -ballVelocity.Y;
	}
}

/*
 * updateBallDataOnPaddleHit(): change ball velocity x & y components based on where ball hit the paddle
 * 
 * @params:
 *		paddleProps - paddle properties
 *		ballProps - ball properties
 *		ballSpeed - ball speed = dist moved per iteration
 *		ballVelocity - ball velocity = components of speed
 */
void Level::updateBallDataOnPaddleHit(const SDL_Rect& paddleProps, const SDL_Rect& ballProps, 
									  int ballSpeed, BallVelocity& ballVelocity)
{
	// Part of paddle hit -> different angle change
	float paddlePart = (paddleProps.x + static_cast<float>(paddleProps.w / 2)) - (ballProps.x + static_cast<float>(ballProps.w / 2));
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

/*
 * fillRenderLevelBuffer(): fill buffer with data to be rendered
 */
void Level::fillRenderLevelBuffer()
{
	// Bricks
	SDL_Texture* brickTexture;
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		// Solid color inside Brick object
		// m_brickObjects.at(i)->fillRenderBricksBuffer();

		if (m_brickObjects.at(i)->getBrickExists())
		{
			// Texture from texture pool
			uint32_t index = m_brickObjects.at(i)->getTextureIndex();
			brickTexture = m_objectTexturePool->getTextureFromVector(index);
			SDL_Rect brickRect = m_brickObjects.at(i)->getBrickProps();
			CHECK_SDL_NEGATIVE_ERROR_NOTHROW(SDL_RenderCopy(m_renderHandle, brickTexture, NULL, &brickRect));
		}
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
	m_totalNumberOfBricks = m_mainAttributesStruct.rowCount * m_mainAttributesStruct.colCount;

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
	for (int i = 0; i < m_totalNumberOfBricks; i++)
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

		// Find first brick ID in layout (first non white space)
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
	int paddleWidthPixels = 100;
	int paddleHeightPixels = 12;
	m_objectPaddle->setSize(paddleWidthPixels, paddleHeightPixels);

	int paddleSpeed = 10;
	m_objectPaddle->setSpeed(paddleSpeed);

	m_paddleInitialPos.x = WINDOW_WIDTH / 2 - paddleWidthPixels/2;
	m_paddleInitialPos.y = WINDOW_HEIGHT - (LOWER_SCREEN_OFFSET_PIXELS + paddleHeightPixels);
	m_objectPaddle->setInitialPosition(m_paddleInitialPos.x, m_paddleInitialPos.y);
}

/*
 * setBallData(): set data needed by Ball object
 */
void Level::setBallData()
{
	int ballWidthPixels = 12;
	int ballHeightPixels = 12;
	m_objectBall->setSize(ballWidthPixels, ballHeightPixels);

	int ballSpeed = 4;
	m_objectBall->setSpeed(ballSpeed);

	// TODO: check where lowest row of bricks is, not window height/2 
	m_ballInitialPos.x = WINDOW_WIDTH / 2 - ballWidthPixels / 2;
	m_ballInitialPos.y = WINDOW_HEIGHT / 2;
	m_objectBall->setInitialPosition(m_ballInitialPos.x, m_ballInitialPos.y);

	m_initialBallVel.X = 0;
	m_initialBallVel.Y = ballSpeed;
	m_objectBall->setVelocity(m_initialBallVel);
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
		std::string idString(1, m_id);

		// Texture path
		const char* textPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_TEXTURE, &textPath) != tinyxml2::XML_SUCCESS)
		{
			// TODO: use default texture
			// textPath =
			LOG_AND_THROW("Unrecognized texture path in file: " + filename + " under: " + idString);
		}
		brickAttStruct.texturePath = textPath;

		// Brick hit sound path
		const char* hitSndPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_HIT_SOUND, &hitSndPath) != tinyxml2::XML_SUCCESS)
		{
			// TODO: use default sound
			LOG_AND_THROW("Unrecognized hit sound path in file: " + filename + " under: " + idString);
		}
		brickAttStruct.hitSoundPath = hitSndPath;

		// Break sound path
		const char* brkSndPath;
		if (pRepeatingElement->QueryStringAttribute(BRICK_BREAK_SOUND, &brkSndPath) != tinyxml2::XML_SUCCESS)
		{
			// TODO: use default sound
			//LOG_AND_THROW("Unrecognized break sound path in file: " + filename + " under: " + idString);
		}
		brickAttStruct.breakSoundPath = brkSndPath;

		// Brick break score
		if (pRepeatingElement->QueryIntAttribute(BRICK_BREAK_SCORE, &brickAttStruct.breakScore) != tinyxml2::XML_SUCCESS)
		{
			// TODO: use default score
			//LOG_AND_THROW("Unrecognized hit pts in file: " + filename + " under: " + idString);
		}

		// Hit points
		if (pRepeatingElement->QueryIntAttribute(BRICK_HIT_PTS, &brickAttStruct.hitPoints) != tinyxml2::XML_SUCCESS)
		{
			// Check if infinite
			const char* maybeInf;
			if (pRepeatingElement->QueryStringAttribute(BRICK_HIT_PTS, &maybeInf) != tinyxml2::XML_SUCCESS)
			{
				// Undefined, TODO: use default num of hit points
				LOG_AND_THROW("Unrecognized brick hit points in file: " + filename + " under: " + idString);
			}
			else
			{
				if (std::string(maybeInf) == INFINITE_1 || std::string(maybeInf) == INFINITE_2)
				{
					brickAttStruct.hitPoints = 0;
					brickAttStruct.isBreakable = false;
					brickAttStruct.breakScore = 0;
				}
			}
			// TODO: use default num of hit points
			//brickAttStruct.hitPoints = hitPts;
			//brickAttStruct.isBreakable = true
		}
		else
		{
			// Success, integer num of hit points
			brickAttStruct.isBreakable = true;
		}

		brickAttStruct.key = m_id; // save for texture locating

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
	Point l1{};
	l1.x = rect1.x;
	l1.y = rect1.y;
	// r1 - bottom right first rect
	Point r1{};
	r1.x = rect1.x + rect1.w;
	r1.y = rect1.y + rect1.h;
	// l2 - top left second rect
	Point l2{};
	l2.x = rect2.x;
	l2.y = rect2.y;
	// r2 - bottom right second rect
	Point r2{};
	r2.x = rect2.x + rect2.w;
	r2.y = rect2.y + rect2.h;

	// TODO: if rectangle has area 0

	// If one rectangle is on left side of other
	if (l1.x > r2.x || l2.x > r1.x)
	{
		return false;
	}

	// If one rectangle is above other
	if (r1.y < l2.y || r2.y < l1.y)
	{
		return false;
	}

	return true;
}

/*
 * getLivesChange(): when life is lost (e.g. ball missed) or gained (e.g. powerups),
 *				share that change with Application which holds game end conditions
 *
 * @return: positive or negative number of lives number changed
 */
int Level::getLivesChange() const
{
	return m_livesNumChanged;
}

/*
 * getScoreChange(): when score is changed, share that change with Application which holds total score between levels
 *
 * @return: positive or negative score change
 */
int Level::getScoreChange() const
{
	return m_scoreChange;
}

/*
 * getLevelName(): get level name
 *
 * @return: level name string
 */
std::string Level::getLevelName() const
{
	return m_levelName;
}

/*
 * getBackgroundPath(): get background texture file path
 *
 * @return: texture file path
 */
std::string Level::getBackgroundPath() const
{
	return m_mainAttributesStruct.backgroundPath;
}

/*
 * setLivesChange(): set number of lives change
 *
 * @params:
 *		lives - lives change
 */
void Level::setLivesChange(int lives)
{
	m_livesNumChanged = lives;
}

/*
 * setScoreChange(): set score change
 *
 * @params:
 *		score - score change
 */
void Level::setScoreChange(int score)
{
	m_scoreChange = score;
}

/*
 * areAllBricksBroken(): returns bool
 *
 * @return: true if all bricks are broken
 */
bool Level::areAllBricksBroken()
{
	return m_allBricksBroken;
}

/*
 * initTexturePool(): creates TexturePool object and adds textures to vector using brick objects info
 *					: uses renderer handle -> should be called after setting renderer handle
 */
void Level::initTexturePool()
{
	// Create bricks texture pool object
	m_objectTexturePool = std::make_unique<TexturePool>();
	SDL_Texture* addTexture;
	
	for (auto const& [key, val] : m_brickTypesMap)
	{
		// Texture path saved per brick type in bricktypesmap
		addTexture = m_objectTexturePool->loadTextureFromFile(m_renderHandle, m_brickTypesMap.at(key).texturePath);
		// Add that texture to texture pool object's member vector and check at which index texture was inserted
		uint32_t index = m_objectTexturePool->addTextureToPool(addTexture);
		// Set that index in every brick object
		m_brickTypesMap.at(key).textureVectorIndex = index;
	}
}

/*
 * initSoundPool(): creates SoundPool object and adds sounds to vector using brick objects info
 */
void Level::initSoundPool()
{
	// Create bricks texture pool object
	// Hit sounds
	m_objectSoundPoolHit = std::make_unique<SoundPool>();
	// Break sound
	m_objectSoundPoolBreak = std::make_unique<SoundPool>();

	Mix_Chunk* addSoundHit;
	Mix_Chunk* addSoundBreak;

	for (auto const& [key, val] : m_brickTypesMap)
	{
		// Sound path saved per brick type in bricktypesmap
		addSoundHit = m_objectSoundPoolHit->loadSoundFromFile(m_brickTypesMap.at(key).hitSoundPath);
		addSoundBreak = m_objectSoundPoolBreak->loadSoundFromFile(m_brickTypesMap.at(key).breakSoundPath);
		// Add that sound to sound pool object's member vector and check at which index texture was inserted
		uint32_t indexHit = m_objectSoundPoolHit->addSoundToPool(addSoundHit);
		uint32_t indexBreak = m_objectSoundPoolBreak->addSoundToPool(addSoundBreak);
		// Set that index in every brick object
		m_brickTypesMap.at(key).hitSoundVectorIndex = indexHit;
		m_brickTypesMap.at(key).breakSoundVectorIndex = indexBreak;
	}
}

/*
 * setBrickTextureIndices(): set index of texture vector (brick texture) for every brick created
 */
void Level::setBrickTextureIndices()
{
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		char key = m_brickObjects.at(i)->getBrickType();
		// Brick types map is mapped with brick type as key
		// Use that key to save index for accessing texture vector
		m_brickObjects.at(i)->setTextureIndex(m_brickTypesMap.at(key).textureVectorIndex);
	}
}

/*
 * setSoundIndicesAndHandle(): set indices of sound vector for every brick created, and set ptr to sound
 */
void Level::setSoundIndicesAndHandle()
{
	for (uint32_t i = 0; i < m_brickObjects.size(); i++)
	{
		char key = m_brickObjects.at(i)->getBrickType();
		// Brick types map is mapped with brick type as key
		// Use that key to save index for accessing texture vector
		m_brickObjects.at(i)->setHitSoundIndex(m_brickTypesMap.at(key).hitSoundVectorIndex);
		m_brickObjects.at(i)->setBreakSoundIndex(m_brickTypesMap.at(key).breakSoundVectorIndex);
		// Use this index to also set actual sound handle
		Mix_Chunk* soundHit		= m_objectSoundPoolHit->getSoundFromVector(m_brickTypesMap.at(key).hitSoundVectorIndex);
		Mix_Chunk* soundBreak	= m_objectSoundPoolBreak->getSoundFromVector(m_brickTypesMap.at(key).breakSoundVectorIndex);
		// Set handles to use in Brick
		m_brickObjects.at(i)->setHitSoundHandle(soundHit);
		m_brickObjects.at(i)->setBreakSoundHandle(soundBreak);
	}
}

