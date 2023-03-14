/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: 
 */
#pragma once

//***************************
// C++ Includes
//***************************

//***************************
// 3rd Party Includes
//***************************
#include "SDL.h"

//***************************
// App Includes
//***************************

//***************************
// Definition
//***************************

class Application
{
	public:
		static Application* getInstance();
		int execute();

	private:
		static Application App;
		Application();
		bool initialize();
		bool destroy();

};
