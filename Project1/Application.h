/*
* Created: Mar 2023
* Description: 
* 
*/

#pragma once

#include "SDL.h"

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
