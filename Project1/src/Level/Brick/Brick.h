/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description:
 */
#pragma once

 //***************************
 // C++ Includes
 //***************************
#include <memory>

 //***************************
 // 3rd Party Includes
 //***************************
#include "SDL2/SDL.h"

 //***************************
 // App Includes
 //***************************
#include "../LevelDataStructures.h"

 //***************************
 // Definition
 //***************************

class Brick
{
	public:
		Brick();
		~Brick();

		// Factory method
		static std::shared_ptr<Brick> makeBrick(BrickAttributes brickAtt, SDL_Rect brickProperties); // static -> can be used before instancing object

	private:
		bool m_doesBrickExist = false;
		BrickAttributes m_brickAttributes{};
};
