/*
 * AnimatedTexture.h
 *
 *  Created on: Apr 12, 2019
 *      Author: dennisgunia
 */

#ifndef ANIMATEDTEXTURE_H_
#define ANIMATEDTEXTURE_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include <sstream>

typedef SDL_Texture* FramePointer;

class AnimatedTexture {
private:
	bool animatedSequence;
	int frames;
	int frame;

	FramePointer *tx_frames;

public:
	AnimatedTexture( );
	void setupAnimatedTexture( std::string file, int frames ,SDL_Renderer *render );
	void doAnimationStep();
	virtual ~AnimatedTexture();

	int getOffset();
	SDL_Texture *getTexture(int offest);

};

#endif /* ANIMATEDTEXTURE_H_ */
