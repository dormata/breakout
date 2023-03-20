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
 */
std::shared_ptr<Level> Level::makeLevel(FileReaderOutputData configStruct)
{
	std::shared_ptr<Level> objectLevel = std::make_shared<Level>();

	// Parse config file
	objectLevel->parseXML(configStruct.configFilePathAndName);

	return objectLevel;
}

/*
 * parseXML():
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
	CHECK(pElement, "XML error parsing element " + elementNameString + "!");
	// Main element attributes
	MainElementAttributes mainAttributesStruct{};
	// 1
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(ROW_COUNT, &mainAttributesStruct.rowCount), doc);
	// 2
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(COL_COUNT, &mainAttributesStruct.colCount), doc);
	// 3
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(ROW_SPACING, &mainAttributesStruct.rowSpacing), doc);
	// 4
	CHECK_TINYXML_ERROR(pElement->QueryIntAttribute(COL_SPACING, &mainAttributesStruct.colSpacing), doc);
	// 5
	CHECK_TINYXML_ERROR(pElement->QueryStringAttribute(BACKGROUND_TEXTURE_PATH, &mainAttributesStruct.backgroundPath), doc);

	// TODO: add defaults ako necega nema u xmlu

	// First child of main element
	tinyxml2::XMLElement* pSubElement1 = pElement->FirstChildElement(FIRST_CHILD_ELEMENT_1);
	elementNameString = FIRST_CHILD_ELEMENT_1;
	CHECK(pElement, "XML error parsing element " + elementNameString + "!");

	// Brick type repeating
	for (tinyxml2::XMLElement* pRepeatingElement = pSubElement1->FirstChildElement(SECOND_CHILD_ELEMENT_1);
		pRepeatingElement != nullptr; pRepeatingElement = pRepeatingElement->NextSiblingElement())
	{
		int score;
		// Missing attribute is not considered error here? Neki defaults?
		pElement->QueryIntAttribute("BreakScore", &score), doc;
	}


	/*tinyxml2::XMLElement* pRepeatingElement = pSubElement1->FirstChildElement(SECOND_CHILD_ELEMENT_1);
	while (pRepeatingElement != nullptr)
	{
		std::cout << "unutra" << std::endl;
		pRepeatingElement->NextSiblingElement();
	}*/
		// NEMA SVAKI BRICK TYPE SVE OVE ATTRIBUTE
	// ovo ovak zbricka komp - mozda dodaj max moguce
}

void Level::printbla()
{
	std::cout << "BLA" << std::endl;
}