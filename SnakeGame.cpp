/*
 * snakeGame.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: dennisgunia
 */

#include "SnakeGame.hpp"





static void soundPlayloop(){
	Mix_PlayMusic(AssetLoader::AL_LoadMusic(assetPath,"8bit Stage1 Loop.wav"),-1);
}

SnakeGame::SnakeGame(size Size, SDL_Renderer *render, SDL_Window *window) {
	this->Size = Size;
	this->render = render;
	this->player = {Size.X/2,Size.Y/2,true};
	this->direction = dir_stop;
	this->taillen = 0;
	this->foodcount = 0;
	this->lastWasHit = false;
	this->speed = 750;
	this->coins = 0;
	this->score = 0;
	this->highscore = 0;
	this->activePowerup = foodtype::powerup_none;
	this->tailPhase = -1;
	this->countdownPowerup = -1;
	this->tmpOffset = {20,20};

	this->stars = AssetLoader::AL_LoadTexture(assetPath,"stars.png",this->render);

	this->powerup[foodtype::powerup_potion] =AssetLoader::AL_LoadTexture(assetPath,"potion_A_green_full.png",this->render);

	//this->font=TTF_OpenFont("/home/dennisgunia/eclipse-workspace-cpp/Desktop/Seven Oh Ess 8x8 Monospaced.ttf", 29);
	this->font=AssetLoader::AL_LoadFont(assetPath,"Seven Oh Ess 8x8 Monospaced.ttf", 29);

	//this->snd_click = Mix_LoadWAV("/home/dennisgunia/eclipse-workspace-cpp/Desktop/Misc01.wav");
	//this->snd_death = Mix_LoadWAV("/home/dennisgunia/eclipse-workspace-cpp/Desktop/Death02.wav");
	//this->snd_powerup = Mix_LoadWAV("/home/dennisgunia/eclipse-workspace-cpp/Desktop/Misc05.wav");

	this->snd_click = AssetLoader::AL_LoadChunk(assetPath,"Misc01.wav");
	this->snd_death = AssetLoader::AL_LoadChunk(assetPath,"Death02.wav");
	this->snd_powerup = AssetLoader::AL_LoadChunk(assetPath,"Misc05.wav");

	SDL_SetWindowSize(window,this->Size.scale * this->Size.X +  offset_left + offset_right,this->Size.scale * this->Size.Y +  offset_top + offset_bottom);

	Mix_VolumeChunk(snd_click, 35);



	//this->tx_food[foodtype::type_coin_std].setupAnimatedTexture("/home/dennisgunia/eclipse-workspace-cpp/Desktop/coin",  14,this->render );
	//this->tx_food[foodtype::type_coin_death].setupAnimatedTexture("/home/dennisgunia/eclipse-workspace-cpp/Desktop/coinDeath",  14,  this->render );
	//this->tx_food[foodtype::type_powerup].setupAnimatedTexture("/home/dennisgunia/eclipse-workspace-cpp/Desktop/sparkle",  34,  this->render );


	this->tx_food[foodtype::type_coin_std].setupAnimatedTexture(AssetLoader::AL_AssetURL(assetPath,"coin"),  14,this->render );
	this->tx_food[foodtype::type_coin_death].setupAnimatedTexture(AssetLoader::AL_AssetURL(assetPath,"coinDeath"),  14,  this->render );
	this->tx_food[foodtype::type_powerup].setupAnimatedTexture(AssetLoader::AL_AssetURL(assetPath,"sparkle"),  34,  this->render );


	this->Music_normal = AssetLoader::AL_LoadMusic(assetPath,"8bit Stage1 Loop.wav");
	this->Music_powerup1 = AssetLoader::AL_LoadMusic(assetPath,"8bit Stage4 Loop.wav");

	text_animations = SnakeTextAnimationHelper();
	text_animations.setRenderer(this->render);

}





void SnakeGame::doAnimation(){
	this->text_animations.doAnimations();
	this->tx_food[0].doAnimationStep();
	this->tx_food[1].doAnimationStep();
	this->tx_food[2].doAnimationStep();

	if (this->tailPhase >= 0){
		this->tailPhase += 20;
		if (this->tailPhase >= 180){
			this->tailPhase = 0;
		}
	}
	if (this->countdownPowerup > 0){
		this->countdownPowerup --;
	}else if (this->countdownPowerup == 0){
		this->countdownPowerup --;
		this->activePowerup = foodtype::powerup_none;
		this->tailPhase = -1;
		this->countdownPowerup = -1;
		Mix_PlayMusic(this->Music_normal, -1);
	}



}

SnakeGame::~SnakeGame() {
	// TODO Auto-generated destructor stub
}

void SnakeGame::shiftTailArray () {
	if (this->taillen > 0){
		// aktuelle länge bestimmen
		int len =this->taillen;
		// array verschieben
		if (this->lastWasHit ){
			//len++;
			this->lastWasHit = false;
		}
		for ( int index = len ; index > 0; index--){
			this->tail[index] = this->tail[index - 1];
		}
		// aktuelle position hinzufügen
		this->tail[0] = this->player;

		this->tail[0].enabled = true;

	}
}

void SnakeGame::addTailArray(){

	this->taillen++;
	this->lastWasHit = true;
	this->speed = this->speed - (this->speed * 0.10);

	if (this->speed < 50){
		this->speed = 50;
	}
	Mix_PlayChannel(-1, snd_click, 0);
}

void SnakeGame::setDirection(int direction){
	this->direction = direction;
	if (direction == dir_stop){
		Mix_PlayChannel(-1, snd_death, 0);
	}
}

void SnakeGame::convertToCoins(){
	for ( int index3 = 0 ; index3 < 500 ; index3++){
		this->food[index3].X = 0;
		this->food[index3].Y = 0;
		this->food[index3].enabled = false;
		this->food[index3].type = foodtype::type_coin_std;
		this->food[index3].offset = 0;
	}
	int arrayLen = this->taillen;
	for ( int index = 0 ; index < arrayLen ; index++){
		if (this->tail[index].enabled){
			this->foodcount++;
			for ( int index2 = 0 ; index2 < 50 ; index2++){
				if (this->food[index2].enabled == false){
					this->food[index2].X = this->tail[index].X;
					this->food[index2].Y = this->tail[index].Y;
					this->food[index2].enabled = true;
					this->food[index2].type = foodtype::type_coin_death;
					//this->food[index2].offset = this->tx_food[foodtype::type_coin_death].getOffset();
					this->food[index2].powerupType = foodtype::powerup_none;
					this->tail[index].enabled = false;
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

}

bool SnakeGame::doSnakeStep(){

	switch(this->direction){
	case dir_up:
		this->shiftTailArray();
		this->player.Y -= 1;
		break;
	case dir_down:
		this->shiftTailArray();
		this->player.Y += 1;
		break;
	case dir_left:
		this->shiftTailArray();
		this->player.X -= 1;
		break;
	case dir_right:
		this->shiftTailArray();
		this->player.X += 1;
		break;
	default:
		return true;
	}

	for ( int index = 0 ; index < 500 ; index++){
		if (this->food[index].enabled && (this->food[index].X == this->player.X)&& (this->food[index].Y == this->player.Y)) {

			std::stringstream popup;
			int tx = this->food[index].X * this->Size.scale + 2 + offset_left;
			int ty = (this->food[index].Y - 1) * this->Size.scale + 2 + offset_top;

			if (this->food[index].powerupType == foodtype::powerup_none){

				this->addTailArray();
				if (this->food[index].type == foodtype::type_coin_std){
					this->score += 20;
					popup << "+20" ;
				}else{
					this->score += 5;
					popup << "+5" ;
				}
				if (this->highscore < this->score){
					this->highscore = this->score;
				}
				this->coins++;

				this->food[index].enabled = false;

				this->text_animations.startAnimation(tx + this->tmpOffset.X,ty+ this->tmpOffset.Y,popup.str(),{64,255,64,0},this->font);

			}else{
				if (this->activePowerup == foodtype::powerup_none){
					this->score += 150;
					popup << "+150" ;
					this->text_animations.startAnimation(tx + this->tmpOffset.X,ty+ this->tmpOffset.Y,popup.str(),{64,255,64,0},this->font);

					this->food[index].enabled = false;
					this->activePowerup = this->food[index].powerupType;
					Mix_PlayChannel(-1, snd_powerup, 0);
					this->tailPhase = 0;
					this->countdownPowerup = 500;
					Mix_PlayMusic(this->Music_powerup1, -1);


				}
			}
		}
	}


	if (this->player.X < 0){
		this->player.X = this->Size.X - 1;
		if (this->tailPhase < 0){return false;};

	}
	if (this->player.Y < 0){
		this->player.Y = this->Size.Y - 1;
		if (this->tailPhase < 0){return false;};
	}
	if (this->player.X > this->Size.X - 1){
		this->player.X = 0;
		if (this->tailPhase < 0){return false;};
	}
	if (this->player.Y > this->Size.Y - 1){
		this->player.Y = 0;
		if (this->tailPhase < 0){return false;};
	}

	for ( int index = 0 ; index < this->taillen + 1 ; index++){
		if (this->tail[index].enabled && (this->tail[index].X == this->player.X)&& (this->tail[index].Y == this->player.Y)){
			if (this->tailPhase < 0){return false;};
		}
	}
	return true;
}

void SnakeGame::generateFood () {

	int foodtype = rand()%40;
	if (foodtype < 30)
	{
		this->foodcount++;
		for ( int index = 0 ; index < 500 ; index++){
			if (this->food[index].enabled == false){
				this->food[index].X = rand()%this->Size.X;
				this->food[index].Y = rand()%this->Size.Y;
				this->food[index].enabled = true;
				this->food[index].type = foodtype::type_coin_std;
				this->food[index].offset = this->tx_food[foodtype::type_coin_std].getOffset();
				this->food[index].powerupType = foodtype::powerup_none;
				break;
			}
		}
	}
	else if (foodtype < 40)
	{
		this->foodcount++;
		for ( int index = 0 ; index < 500 ; index++){
			if (this->food[index].enabled == false){
				this->food[index].X = rand()%this->Size.X;
				this->food[index].Y = rand()%this->Size.Y;
				this->food[index].enabled = true;
				this->food[index].type = foodtype::type_powerup;
				this->food[index].offset = this->tx_food[foodtype::type_powerup].getOffset();
				this->food[index].powerupType = foodtype::powerup_potion;

				break;
			}
		}
	}
}

int SnakeGame::getTimeOut(){
	return this->speed;
}


void SnakeGame::drawHud(){


}


void SnakeGame::drawGame(){


	coordinate playerInRender = {20,20};
	SDL_Rect cell;
	SDL_GetRendererOutputSize(this->render,&cell.w,&cell.h);


	playerInRender.X = this->player.X * this->Size.scale + 2 + offset_left + this->tmpOffset.X;
	playerInRender.Y = this->player.Y * this->Size.scale + 2 + offset_top + this->tmpOffset.Y;

	int steps = 4;


	if(playerInRender.X < cell.w * 0.2){
		this->tmpOffset.X += this->Size.scale /steps;
	}
	if(playerInRender.X > cell.w * 0.8){
		this->tmpOffset.X -= this->Size.scale /steps;
	}

	if(playerInRender.Y < cell.h * 0.2){
		this->tmpOffset.Y += this->Size.scale /steps;
	}
	if(playerInRender.Y > cell.h * 0.8){
		this->tmpOffset.Y -= this->Size.scale/steps;
	}

	SDL_SetRenderDrawColor(this->render, SNAKE_BACKGROUND);
	SDL_RenderClear(this->render);



	this->drawStarfield(this->tmpOffset.X / 8,this->tmpOffset.Y / 8);


	for (int posY = 0 ; posY < this->Size.Y + 1;posY++){
		SDL_Rect cell;
		cell.x = offset_left - 32  + offset_left - 32 + this->tmpOffset.X;
		cell.y = posY * this->Size.scale+ offset_top - 32 + this->tmpOffset.Y;
		cell.h = this->Size.scale;
		cell.w = this->Size.scale;
		SDL_SetRenderDrawColor(this->render, SNAKE_BORDER);
		SDL_RenderDrawRects(this->render,&cell,1);
		cell.x = (this->Size.X + 1) * this->Size.scale  + offset_left - 32 + this->tmpOffset.X;
		cell.y = posY * this->Size.scale+ offset_top - 32 + this->tmpOffset.Y;
		SDL_RenderDrawRects(this->render,&cell,1);
	}
	for (int posX = 0 ; posX < this->Size.X + 1;posX++){
		SDL_Rect cell;
		cell.x = posX * this->Size.scale + offset_left - 32 + this->tmpOffset.X;
		cell.y = offset_top - 32 + this->tmpOffset.Y;
		cell.h = this->Size.scale;
		cell.w = this->Size.scale;
		SDL_SetRenderDrawColor(this->render, SNAKE_BORDER);
		SDL_RenderDrawRects(this->render,&cell,1);

		cell.x = posX * this->Size.scale + offset_left - 32 + this->tmpOffset.X;
		cell.y = (this->Size.Y +1) * this->Size.scale + offset_top - 32 + this->tmpOffset.Y;
		SDL_RenderDrawRects(this->render,&cell,1);
	}


	SDL_SetRenderDrawColor(this->render, SNAKE_PLAYER);

	cell.x = this->player.X * this->Size.scale + 2 + offset_left + this->tmpOffset.X;
	cell.y = this->player.Y * this->Size.scale + 2 + offset_top + this->tmpOffset.Y;
	cell.h = this->Size.scale - 4;
	cell.w = this->Size.scale - 4;


	if (this->tailPhase >= 0){
		int phase = (this->tailPhase)%180;
		SDL_SetRenderDrawColor(this->render, 255,128,000,(sin(phase*PI/180)*255));
	}
	SDL_RenderFillRects(this->render,&cell,1);

	int arrayLen = taillen;

	SDL_SetRenderDrawColor(this->render, SNAKE_TAIL);
	SDL_Rect cell1;
	cell1.h = this->Size.scale - 4;
	cell1.w = this->Size.scale - 4;
	SDL_SetRenderDrawColor(this->render, SNAKE_TAIL);
	for ( int index = 0 ; index < arrayLen ; index++){
		if (this->tail[index].enabled){
			cell1.x = this->tail[index].X * this->Size.scale + 2 + offset_left + this->tmpOffset.X;
			cell1.y = this->tail[index].Y * this->Size.scale + 2 + offset_top + this->tmpOffset.Y;

			if (this->tailPhase >= 0){
				int offset = (index + 1) * 20;
				int phase = (offset + this->tailPhase)%180;
				SDL_SetRenderDrawColor(this->render, 255,255,255,(sin(phase*PI/180)*255));
			}


			SDL_RenderFillRects(this->render,&cell1,1);
		}
		else
		{
			break;
		}
	}

	//SDL_SetRenderDrawColor(this->render, SNAKE_FOOD);
	for ( int index = 0 ; index < 500 ; index++){
		if (this->food[index].enabled){
			//SDL_Rect cell1;
			cell1.x = this->food[index].X * this->Size.scale + 2 + offset_left + this->tmpOffset.X;
			cell1.y = this->food[index].Y * this->Size.scale + 2 + offset_top + this->tmpOffset.Y;
			cell1.h = this->Size.scale - 4;
			cell1.w = this->Size.scale - 4;

			//SDL_RenderFillRects(this->render,&cell1,1);

			if(this->food[index].type == foodtype::type_powerup){
				switch(this->food[index].powerupType) {
				case foodtype::powerup_potion:
					SDL_RenderCopy(this->render, this->powerup[this->food[index].powerupType], NULL, &cell1);

					break;
				default:
					break;
				}
			}
			//SDL_RenderCopy(this->render, this->coin[(this->coinStep + this->food[index].offset)%14][this->food[index].type -1], NULL, &cell1);

			//printf("[debug] getting texture for %i with animIndex %i - pointer to %i\n",index,this->food[index].offset,this->tx_food[this->food[index].type].getTexture(this->food[index].offset));

			SDL_RenderCopy(this->render, this->tx_food[this->food[index].type].getTexture(this->food[index].offset), NULL, &cell1);

		}
	}

	SDL_Color color={255,255,255,255}, bgcolor={0,0,0,0};
	std::stringstream ss;
	ss << "score:" << this->score;
	std::string s = ss.str();
	char * tab2 = new char [s.length()+1];
	strcpy (tab2, s.c_str());
	// text score
	SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font,tab2,color);
	SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_Rect txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = 10;
	txt_score_dest.y = 1;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	// text highscore
	std::stringstream ss2;
	ss2 << "highscore:" << this->highscore;
	std::string s2 = ss2.str();
	char * tab3 = new char [s2.length()+1];
	strcpy (tab3, s2.c_str());
	temp_surface = TTF_RenderText_Blended(this->font,tab3,color);
	SDL_Texture *tx_highscore = SDL_CreateTextureFromSurface(this->render, temp_surface);
	txt_score_dest.x = txt_score_dest.w + 50;
	SDL_QueryTexture(tx_highscore, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	SDL_RenderCopy(this->render, tx_highscore, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_highscore);
	SDL_FreeSurface(temp_surface);
	// text coins
	std::stringstream ss3;
	ss3 << "coins:" << this->coins;
	std::string s3 = ss3.str();
	char * tab4 = new char [s3.length()+1];
	strcpy (tab4, s3.c_str());
	temp_surface = TTF_RenderText_Blended(this->font,tab4,color);
	SDL_Texture *tx_coins = SDL_CreateTextureFromSurface(this->render, temp_surface);
	txt_score_dest.x =txt_score_dest.x + txt_score_dest.w + 50;
	SDL_QueryTexture(tx_coins, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	SDL_RenderCopy(this->render, tx_coins, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_coins);
	SDL_FreeSurface(temp_surface);
	this->text_animations.renderAnimations();
	SDL_RenderPresent(this->render);


}

int SnakeGame::getScore(){
	return this->score;
}

void SnakeGame::startGame(){

	Mix_PlayMusic(AssetLoader::AL_LoadMusic(assetPath,"8bit Stage1 Intro.wav"), 1);
	Mix_HookMusicFinished(soundPlayloop);
	this->taillen = 0;
	this->speed = 300;// 750;
	this->player = {this->Size.X/2,this->Size.Y/2,true};
	this->score = 0;
	this->coins = 0;
	this->activePowerup = foodtype::powerup_none;
	this->tailPhase = -1;
	SDL_Rect cell;
		SDL_GetRendererOutputSize(this->render,&cell.w,&cell.h);
	this->tmpOffset.X = (cell.w/2) - (((this->player.X)+2) * this->Size.scale);
	this->tmpOffset.Y = (cell.h/2) - (((this->player.Y)+2) * this->Size.scale);
}

void SnakeGame::drawStarfield(int x, int y){
	SDL_Rect cell;


	SDL_GetRendererOutputSize(this->render,&cell.w,&cell.h);
	x = x%cell.w;
	y = y%cell.h;
	cell.x = x;
	cell.y = y;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x;
	cell.y = y - cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x;
	cell.y = y + cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x + cell.w;
	cell.y = y - cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x+ cell.w;
	cell.y = y + cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x - cell.w;
	cell.y = y - cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x - cell.w;
	cell.y = y + cell.h;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x + cell.w;
	cell.y = y;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = x - cell.w;
	cell.y = y;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
}
