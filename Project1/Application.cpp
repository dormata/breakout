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

//***************************
// App Includes
//***************************
#include "Application.h"
#include "Logger.h"

//***************************
// Implementation
//***************************

Application Application::App;

Application::Application()
{

}

bool Application::initialize()
{
	CHECK_SDL_NEGATIVE_ERROR(SDL_Init(SDL_INIT_EVERYTHING));

	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	CHECK_SDL_FALSE_ERROR(window);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	CHECK_SDL_FALSE_ERROR(renderer);

	//CHECK_SDL_ENUM_ERROR(SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255));
	CHECK_SDL_NEGATIVE_ERROR(SDL_SetRenderDrawColor(nullptr, 0, 255, 0, 255));

	CHECK_SDL_NEGATIVE_ERROR(SDL_RenderClear(renderer));

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	std::cout << "test" << std::endl;

	return true;
}

int Application::execute()
{
	if (!initialize())
	{
		std::cout << "log error" << std::endl;
	}

	return 1;
}

Application* Application::getInstance()
{ 
	return &Application::App;
}
