/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
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
	std::string fullPathAndName = std::string(RESOURCE_FILES_PATH) + std::string(LEVEL_CONFIG_FOLDER_NAME);
	if (std::filesystem::is_directory(fullPathAndName))
	{
		// Read all available level config files
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator{ fullPathAndName })
		{
			std::string path = entry.path().string();
			m_configFileNames.push_back(path);
		}

		// TODO: uzmi do tocke da dobijes samo brojeve
		// napravi diff, ako je negdje razl od 1, daj warning
		// provjeri i jel negativan broj
	}
	else
	{
		LOG_AND_THROW("Missing resource folder, should be: " + std::string(RESOURCE_FILES_PATH));
	}

	if (m_configFileNames.size() <= 0)
	{
		LOG_AND_THROW("No level config files found in " + fullPathAndName + ", ");
	}

	// debug
	for (uint32_t i = 0; i < m_configFileNames.size(); i++)
	{
		std::cout << m_configFileNames.at(i) << std::endl;
	}
}

/*
 * getConfigFileNames(): get configuration file names
 * 
 * @return: config filenames
 */
std::vector<std::string> FileReader::getConfigFileNames() const
{
	return m_configFileNames;
}

/*
 * ~FileReader(): destroy and release
 */
FileReader::~FileReader()
{

}