/*
 * SnakeTextAnimationHelper.h
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */
#include "SnakeTextAnimation.h"
#ifndef SNAKETEXTANIMATIONHELPER_H_
#define SNAKETEXTANIMATIONHELPER_H_

#define maxAnimations 50

class SnakeTextAnimationHelper {
private:
	SnakeTextAnimation animation[maxAnimations];
	bool animationActive[maxAnimations];
	SDL_Renderer *render;

public:
	SnakeTextAnimationHelper();
	virtual ~SnakeTextAnimationHelper();


	bool startAnimation(int x, int y,std::string s,SDL_Color color,TTF_Font *font);
	void doAnimations();
	void renderAnimations();
	void setRenderer(	SDL_Renderer *render);
};

#endif /* SNAKETEXTANIMATIONHELPER_H_ */
