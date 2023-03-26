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

class Ball : public IDynamicObject
{
	public:
		Ball();
		~Ball();

		void fillRenderBuffer() override;
		void updatePosition();
		void setVelocity(BallVelocity ballVelocity);

		BallVelocity getVelocity() const;

	private:
		BallVelocity m_ballVelocity{};
};
