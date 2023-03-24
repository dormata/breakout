/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: File reading header
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <vector>

 //***************************
 // 3rd Party Includes
 //***************************

 //***************************
 // App Includes
 //***************************
#include "../common/DataStructures/CommonDataStructures.h"

 //***************************
 // Definition
 //***************************

class FileReader
{
public:
	FileReader();
	~FileReader();

	std::vector<FileReaderOutputData> getConfigFileNames() const;

private:
	void readFilenamesInDir();

	static constexpr char LEVEL_CONFIG_FOLDER_NAME[] = "levelConfig/";
	std::vector<FileReaderOutputData> m_configFilesStruct;

};