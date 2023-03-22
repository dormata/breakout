/*
 * Created on: Mar 2023
 * Author: Dora Matic
 * Description: 
 */

 //***************************
 // C++ Includes
 //***************************

//***************************
// 3rd Party Includes
//***************************

//***************************
// App Includes
//***************************
#include "Brick.h"

//***************************
// Implementation
//***************************

/*
 * Level(): initialize
 */
Brick::Brick()
{

}

/*
 * ~Level(): release and destroy
 */
Brick::~Brick()
{

}

/*
 * makeLevel(): factory method for creating multiple objects of class Brick
 * 
 * @params:
 *		brickAtt - structure required to initialize object brick
 *		brickProperties - size and location of brick
 * 
 * @return: pointer to brick object
 */
std::shared_ptr<Brick> Brick::makeBrick(BrickAttributes brickAtt, SDL_Rect brickProperties)
{
	std::shared_ptr<Brick> objectBrick = std::make_shared<Brick>();

	return objectBrick;
}