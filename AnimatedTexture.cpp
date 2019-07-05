/*
 * AnimatedTexture.cpp
 *
 *  Created on: Apr 12, 2019
 *      Author: dennisgunia
 */

#include "AnimatedTexture.h"

void AnimatedTexture::setupAnimatedTexture( std::string file, int frames, SDL_Renderer *render ) {
	// TODO Auto-generated constructor stub
	this->frames = frames;

	this->tx_frames = new FramePointer[frames];
	if (frames > 1){
		this->animatedSequence = true;
	}else{
		this->animatedSequence = false;
	}
	// load textures

	for (int i_frame = 0; i_frame < frames ; i_frame++){
		std::stringstream tx_filenameStream;
		tx_filenameStream << file << "/frame" << i_frame << ".png";
		std::string tx_filenameString = tx_filenameStream.str();
		char * tx_filename = new char [tx_filenameString.length()+1];
		strcpy (tx_filename, tx_filenameString.c_str());

		printf("Loading texture : %s \n", tx_filename);
		this->tx_frames[i_frame] = IMG_LoadTexture(render, tx_filename);
		if (this->tx_frames[i_frame] == NULL ){
			printf("[ERROR] Loading texture : %s failed \n", tx_filename);
		}
	}
}
AnimatedTexture::AnimatedTexture() {
	// TODO Auto-generated destructor stub
}
AnimatedTexture::~AnimatedTexture() {
	// TODO Auto-generated destructor stub
}

void AnimatedTexture::doAnimationStep(){
	this->frame ++;
	if(this->frame >= this->frames){
		this->frame = 0;
	}
};

int AnimatedTexture::getOffset() {
	return this->frame;
}

SDL_Texture *AnimatedTexture::getTexture(int offset) {

	return this->tx_frames[(offset+(this->frame))%(this->frames)];
}
