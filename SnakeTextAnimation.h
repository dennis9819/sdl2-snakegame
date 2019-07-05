/*
 * SnakeTextAnimation.h
 *
 *  Created on: Apr 12, 2019
 *      Author: dennisgunia
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include <sstream>

#ifndef SNAKETEXTANIMATION_H_
#define SNAKETEXTANIMATION_H_

struct textAnimFrame {
	int offset;
	int opacity;
};

#define keyframes 34
class SnakeTextAnimation {
private:
	textAnimFrame keyframe[keyframes];
	int x;
	int y;
	int frames;
	int frame;
	std::string s;
	SDL_Texture *tx_temp;
	SDL_Rect dst_temp;
	SDL_Color color;

public:
	SnakeTextAnimation();
	virtual ~SnakeTextAnimation();

	void addAnimationStep(int index,textAnimFrame frame);
	void startAnimation(int x, int y,std::string s,SDL_Color color,TTF_Font *font, SDL_Renderer *render);
	bool doAnimation();
	void renderAnimation(SDL_Renderer *render);
};

#endif /* SNAKETEXTANIMATION_H_ */
