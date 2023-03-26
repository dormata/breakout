/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Reads file list from specified config directory. DOES NOT PARSE.
 */

//***************************
// C++ Includes
//***************************
#include <filesystem>
#include <iostream>

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "FileReader.h"
#include "../common/DataPaths.h"
#include "../common/Logger.h"

//***************************
// Implementation
//***************************

/*
 * FileReader(): initialize
 */
FileReader::FileReader()
{
	readFilenamesInDir();
}

/*
 * ~readFilenamesInDir(): destroy and release
 */
void FileReader::readFilenamesInDir()
{
	// Assumes directory structure:
	// 1.xml
	// 2.xml
	// ...
	// n.xml
	// where each file name represents one order number of level
	// TODO: more error checks, extension checking...

	std::string fullPathAndName = std::string(RESOURCE_FILES_PATH) + std::string(LEVEL_CONFIG_FOLDER_NAME);
	if (std::filesystem::is_directory(fullPathAndName))
	{
		FileReaderOutputData fileStruct{};
		// Read all available level config files
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator{ fullPathAndName })
		{
			fileStruct.configFilePathAndName = entry.path().string();
			std::string fileName = entry.path().stem().string();
			fileStruct.levelName = fileName;

			m_configFilesStruct.push_back(fileStruct);
		}

		// TODO: uzmi do tocke da dobijes samo brojeve
		// napravi diff, ako je negdje razl od 1, daj warning
		// provjeri i jel negativan broj
	}
	else
	{
		LOG_AND_THROW("Missing resource folder, should be: " + std::string(RESOURCE_FILES_PATH));
	}

	if (m_configFilesStruct.size() <= 0)
	{
		LOG_AND_THROW("No level config files found in " + fullPathAndName + ", ");
	}
}

/*
 * getConfigFileNames(): get configuration file struct
 * 
 * @return: config struct
 */
std::vector<FileReaderOutputData> FileReader::getConfigFileNames() const
{
	return m_configFilesStruct;
}

/*
 * ~FileReader(): destroy and release
 */
FileReader::~FileReader()
{

}