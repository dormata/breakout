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
#include "../../common/Logger.h"

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

		void setRendererHandle(SDL_Renderer* renderHandle);
		void fillRenderBricksBuffer();
		void setBrickExists(bool brickExists);
		void setTextureIndex(uint32_t index);
		void onHit();

		SDL_Rect	getBrickProps() const;
		bool		getBrickExists() const;
		bool		getBrickBreakable() const;
		int			getBrickScore() const;
		std::string	getTexturePath() const;
		uint32_t	getTextureIndex() const;
		char		getBrickType() const;

	private:
		bool			m_doesBrickExist = false;
		SDL_Rect		m_brickRect{};
		BrickAttributes m_brickAttributes{};
		SDL_Renderer*	m_renderHandle = nullptr;
		int				m_hitsLeftToBreak = 0;
		int				m_scoreOnHit = 0; // gets value when brick is broken

		void setBrickAtts(BrickAttributes brickAtts);
		void setBrickRect(SDL_Rect brickRect);
		void setHitsLeft(int hitsLeft);
};
