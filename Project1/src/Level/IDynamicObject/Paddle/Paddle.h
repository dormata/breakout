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

 //***************************
 // App Includes
 //***************************
#include "../IDynamicObject.h"

 //***************************
 // Definition
 //***************************

class Paddle : public IDynamicObject
{
	public:
		Paddle();
		~Paddle();

		void movePaddle(const uint8_t* keystates);
		void fillRenderBuffer() override;

	private:
		uint8_t*		m_keystates = nullptr;
};
