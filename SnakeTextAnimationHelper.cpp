/*
 * SnakeTextAnimationHelper.cpp
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */

#include "SnakeTextAnimationHelper.h"

SnakeTextAnimationHelper::SnakeTextAnimationHelper() {
	// TODO Auto-generated constructor stub
	for (int index = 0; index < maxAnimations ; index++){
		this->animation[index].addAnimationStep(33,{-17,16});
		this->animation[index].addAnimationStep(32,{-16,32});
		this->animation[index].addAnimationStep(31,{-15,48});
		this->animation[index].addAnimationStep(30,{-14,64});
		this->animation[index].addAnimationStep(29,{-13,92});
		this->animation[index].addAnimationStep(28,{-12,128});
		this->animation[index].addAnimationStep(27,{-11,144});
		this->animation[index].addAnimationStep(26,{-10,160});
		this->animation[index].addAnimationStep(25,{-9,176});
		this->animation[index].addAnimationStep(24,{-8,192});
		this->animation[index].addAnimationStep(23,{-7,208});
		this->animation[index].addAnimationStep(22,{-6,224});
		this->animation[index].addAnimationStep(21,{-5,240});
		this->animation[index].addAnimationStep(20,{-4,255});
		this->animation[index].addAnimationStep(19,{-3,255});
		this->animation[index].addAnimationStep(18,{-2,255});
		this->animation[index].addAnimationStep(17,{-1,255});
		this->animation[index].addAnimationStep(16,{0,255});
		this->animation[index].addAnimationStep(15,{1,255});
		this->animation[index].addAnimationStep(14,{2,240});
		this->animation[index].addAnimationStep(13,{3,224});
		this->animation[index].addAnimationStep(12,{4,208});
		this->animation[index].addAnimationStep(11,{5,192});
		this->animation[index].addAnimationStep(10,{6,176});
		this->animation[index].addAnimationStep(9,{7,160});
		this->animation[index].addAnimationStep(8,{8,144});
		this->animation[index].addAnimationStep(7,{9,128});
		this->animation[index].addAnimationStep(6,{10,112});
		this->animation[index].addAnimationStep(5,{11,96});
		this->animation[index].addAnimationStep(4,{12,80});
		this->animation[index].addAnimationStep(3,{13,64});
		this->animation[index].addAnimationStep(2,{14,48});
		this->animation[index].addAnimationStep(1,{15,32});
		this->animation[index].addAnimationStep(0,{16,16});
	}
	this->render = NULL;
}

SnakeTextAnimationHelper::~SnakeTextAnimationHelper() {
	// TODO Auto-generated destructor stub
}

bool SnakeTextAnimationHelper::startAnimation(int x, int y,std::string s,SDL_Color color,TTF_Font *font){
	for (int index = 0; index < maxAnimations ; index++){
		if (this->animationActive[index] == false){
			this->animationActive[index] = true;
			this->animation[index].startAnimation(x,y,s,color,font,this->render);
			return true;
		}
	}
	return false;
}
void SnakeTextAnimationHelper::doAnimations(){
	for (int index = 0; index < maxAnimations ; index++){
		if (this->animationActive[index] == true){
			if(!this->animation[index].doAnimation()){
				this->animationActive[index] = false;
			}
		}
	}
}
void SnakeTextAnimationHelper::renderAnimations(){
	for (int index = 0; index < maxAnimations ; index++){
		if (this->animationActive[index] == true){
			this->animation[index].renderAnimation(this->render);
		}
	}
}

void SnakeTextAnimationHelper::setRenderer(SDL_Renderer *render){
	this->render = render;
}
