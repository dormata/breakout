/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: Error logging class
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <string>

 //***************************
 // 3rd Party Includes
 //***************************

 //***************************
 // App Includes
 //***************************

 //***************************
 // Definition
 //***************************

class Logging
{
	public:
		Logging();
		~Logging();

		void reportLog(std::string expr, std::string fileName, std::string line, bool shouldThrow);

	private:
		std::string getDateTimeString();
};