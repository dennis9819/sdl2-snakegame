/*
 * SnakeTextAnimation.cpp
 *
 *  Created on: Apr 12, 2019
 *      Author: dennisgunia
 */

#include "SnakeTextAnimation.h"

SnakeTextAnimation::SnakeTextAnimation() {
	// TODO Auto-generated constructor stub
	this->frame = -1;
	this->frames = 30;
	this->x = -1;
	this->y = -1;
}

SnakeTextAnimation::~SnakeTextAnimation() {
	// TODO Auto-generated destructor stub
}

void SnakeTextAnimation::addAnimationStep(int index,textAnimFrame frame){
	this->keyframe[index] = frame;
}

void SnakeTextAnimation::startAnimation(int x, int y,std::string s,SDL_Color color,TTF_Font *font, SDL_Renderer *render){
	this->x = x;
	this->y = y;
	this->s = s;
	this->frame = 0;
	this->color = color;

	printf("start animation at x:%i y:%i\n",this->x,this->y);

	char * temp = new char [this->s.length()+1];
	strcpy (temp, this->s.c_str());
	SDL_Surface *temp_surface = TTF_RenderText_Blended(font,temp,this->color);
	this->tx_temp = SDL_CreateTextureFromSurface(render, temp_surface);
	SDL_FreeSurface(temp_surface);

	this->dst_temp.x = this->x;
	this->dst_temp.y = this->y;
	this->color.a = this->keyframe[frame].opacity;
	SDL_QueryTexture(this->tx_temp, NULL, NULL, &this->dst_temp.w, &this->dst_temp.h);
}
bool SnakeTextAnimation::doAnimation(){
	if (this->frame >= 0){
		this->frame ++;
		if (this->frame >= this->frames){
			this->frame = -1;
			SDL_DestroyTexture(this->tx_temp);
			return false;
		}
		return true;
	}else{
		return false;
	}
}


void SnakeTextAnimation::renderAnimation(SDL_Renderer *render){
	if (this->frame >= 0){

		this->dst_temp.y = this->y + this->keyframe[frame].offset;
		SDL_SetTextureAlphaMod( this->tx_temp,this->keyframe[frame].opacity);
		SDL_RenderCopy(render, this->tx_temp, NULL, &this->dst_temp);

	}

}
