/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
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

 //***************************
 // Definition
 //***************************

class FileReader
{
public:
	FileReader();
	~FileReader();

	std::vector<std::string> getConfigFileNames() const;

private:
	void readFilenamesInDir();

	static constexpr char LEVEL_CONFIG_FOLDER_NAME[] = "levelConfig/";
	std::vector<std::string> m_configFileNames;

};