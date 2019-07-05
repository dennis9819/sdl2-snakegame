/*
 * SnakeMenu.cpp
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */

#include "SnakeMenu.h"
#include "global.h"

#include <iostream>
#include <fstream>

SnakeMenu::SnakeMenu(SDL_Renderer *render) {
	// TODO Auto-generated constructor stub
	this->render = render;

	this->font_title = AssetLoader::AL_LoadFont(assetPath,"Seven Oh Ess 8x8 Monospaced.ttf",60);
	this->font_subtitle = AssetLoader::AL_LoadFont(assetPath,"Seven Oh Ess 8x8 Monospaced.ttf",29);
	this->font_text = AssetLoader::AL_LoadFont(assetPath,"Seven Oh Ess 8x8 Monospaced.ttf",20);

	this->music = AssetLoader::AL_LoadMusic(assetPath,"music/Title.wav");
	this->snd_enter =  AssetLoader::AL_LoadChunk(assetPath,"music/UI03.wav");
	this->snd_select = AssetLoader::AL_LoadChunk(assetPath,"music/UI04.wav");

	this->stars = AssetLoader::AL_LoadTexture(assetPath,"stars.png",this->render);
	this->moon = AssetLoader::AL_LoadTexture(assetPath,"moon_surface.png",this->render);
	this->earth = AssetLoader::AL_LoadTexture(assetPath,"earth.png",this->render);
	this->splash = AssetLoader::AL_LoadTexture(assetPath,"splash.png",this->render);

	this->currentFrame = frame_splash;
	this->selectedItem = 0;

	this->splashTimeout = 50;
	this->splashFade = 25;



	this->name = "dennisgunia";

	this->curs_char = ' ';

	this->earthPhase = 0.75;

	Mix_PlayMusic(music,-1);



	this->readScores();

	SDL_StopTextInput();
}

SnakeMenu::~SnakeMenu() {
	// TODO Auto-generated destructor stub
}

void SnakeMenu::pressKey(Menukey key){
	if (this->currentFrame == frame_title){
		switch (key){
		case menukey_up:
			this->selectedItem --;
			Mix_PlayChannel(-1, this->snd_select, 0);
			break;
		case menukey_down:
			this->selectedItem ++;
			Mix_PlayChannel(-1, this->snd_select, 0);
			break;
		case menukey_enter:

			switch(this->selectedItem){
			case btn_exit:

				this->currentFrame = frame_title_fadeout_exit;
				Mix_PlayChannel(-1, this->snd_enter, 0);
				Mix_FadeOutMusic(1000);
				break;
			case btn_start:

				this->currentFrame = frame_nameinput;
				SDL_StartTextInput();
				Mix_PlayChannel(-1, this->snd_enter, 0);
				//Mix_FadeOutMusic(1000);
				break;
			case btn_score:
				this->currentFrame = frame_scoreboard;
				Mix_PlayChannel(-1, this->snd_enter, 0);
				break;

			default:
				break;
			}

			break;
		default:
			break;
		}

		if (this->selectedItem < 0){
			this->selectedItem += 3;
		}
		if (this->selectedItem > 2){
			this->selectedItem -= 3;
		}
	}else if (this->currentFrame == frame_scoreboard){
		switch (key){
		case menukey_enter:
			this->currentFrame = frame_title;
			Mix_PlayChannel(-1, this->snd_enter, 0);
			break;
		default:
			break;
		}

	}else if (this->currentFrame == frame_nameinput){
		switch (key){
		case menukey_up:
			this->selectedItem --;
			Mix_PlayChannel(-1, this->snd_select, 0);


			break;
		case menukey_down:
			this->selectedItem ++;
			Mix_PlayChannel(-1, this->snd_select, 0);
			break;
		case menukey_back:
			if((this->name.length() > 0) && (this->currentFrame == frame_nameinput)){
				this->name.pop_back();
			}
			break;
		case menukey_enter:

			switch(this->selectedItem){
			case 2:
				this->currentFrame = frame_title;
				Mix_PlayChannel(-1, this->snd_enter, 0);
				break;
			case 1:

				this->currentFrame = frame_title_fadeout_start;
				Mix_PlayChannel(-1, this->snd_enter, 0);
				Mix_FadeOutMusic(1000);
				break;


			default:
				break;
			}

			break;
		default:
			break;
		}

		if (this->selectedItem < 0){
			this->selectedItem += 3;
		}
		if (this->selectedItem > 2){
			this->selectedItem -= 3;
		}

		if(this->selectedItem == 0){
			SDL_StartTextInput();
		}else{
			SDL_StopTextInput();
			this->curs_char = ' ';
		}

	}
}

int SnakeMenu::renderFrame(){
	SDL_RenderClear(this->render);
	SDL_Rect cell;
	SDL_GetRendererOutputSize(this->render,&cell.w,&cell.h);

	Uint32 now = SDL_GetTicks();
	if (this->lastAnimFrame + 150 < now){
		this->skyBoxPos += 1;
		if(this->skyBoxPos >= cell.w){
			this->skyBoxPos = 0;
		}




	}
	if (this->lastCurs + 400 < now && this->selectedItem == 0){
		if(this->curs_char == ' '){
			this->curs_char = '_';
		}else{
			this->curs_char = ' ';
		}
		this->lastCurs  = now;
	}



	if (this->lastBlend +30 < now){
		this->lastBlend  = now;
		this->earthPhase += 0.0005;
		if(this->earthPhase >= 2){
			this->earthPhase = 0.00;
		}

		if (this->splashFade > -1 ){
			if(this->splashTimeout > 0 ){
				this->splashTimeout --;
			}else if(this->splashTimeout == 0 ){
				this->currentFrame = frame_title;
				this->splashTimeout --;
			}else{
				if(this->splashFade > 0 ){
					this->splashFade --;
				}else if (this->splashFade == 0 ){

					this->splashFade --;

				}
			}
		}
	}
	cell.x = + this->skyBoxPos;
	cell.y = 0;


	//calculate earth





	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x -= cell.w;
	SDL_RenderCopy(this->render, this->stars, NULL, &cell);
	cell.x = 0;
	SDL_RenderCopy(this->render, this->moon, NULL, &cell);



	SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font_title,"SpaceSnake",{255,255,255,255});
	SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_Rect txt_score_dest;

	const int amplitude = cell.h * 1.75;

	SDL_QueryTexture(this->earth, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (cell.w / 2)-(txt_score_dest.w / 2) - (sin(this->earthPhase * PI)*amplitude);
	txt_score_dest.y = cell.h  * 1.8+ (cos(this->earthPhase * PI)*amplitude);
	SDL_RenderCopy(this->render, this->earth, NULL, &txt_score_dest);


	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (cell.w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 50;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);
	temp_surface = TTF_RenderText_Blended(this->font_subtitle,"(c) 2019 by Dennis Vincent Gunia",{255,255,255,255});
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (cell.w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 130;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);




	if(this->currentFrame == frame_title){
		this->renderMenuButtons();
	}
	if(this->currentFrame == frame_scoreboard){
		this->renderMenuScore();
	}
	if(this->currentFrame == frame_nameinput){
		this->renderNameInput();
	}



	if(!Mix_PlayingMusic()){
		if (this->currentFrame == frame_title_fadeout_exit){
			this->currentFrame = frame_can_exit;
		}
		if (this->currentFrame == frame_title_fadeout_start){
			this->currentFrame = frame_can_start;
		}
	}

	if(this->splashFade >= 0 ){
		cell.x = 0;
		cell.y = 0;
		SDL_GetRendererOutputSize(this->render,&cell.w,&cell.h);
		SDL_SetTextureAlphaMod( this->splash,this->splashFade*10 + 5);
		SDL_RenderCopy(this->render, this->splash, NULL, &cell);

	}

	SDL_RenderPresent(this->render);
	return this->currentFrame;


}

void SnakeMenu::renderMenuButtons(){

	SDL_Color col[3] = {{255,255,255,255},{255,255,255,255},{255,255,255,255}};

	col[this->selectedItem] = {0,255,255,255};
	int scr_w;
	SDL_GetRendererOutputSize(this->render,&scr_w,NULL);
	SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font_subtitle,"play game",col[btn_start]);
	SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_Rect txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 400;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	temp_surface = TTF_RenderText_Blended(this->font_subtitle,"scoreboard",col[btn_score]);
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 450;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	temp_surface = TTF_RenderText_Blended(this->font_subtitle,"exit",col[btn_exit]);
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 600;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);
}

void SnakeMenu::renderMenuScore(){
	int scores = this->scoreboard.size();
		int scr_w;
		SDL_GetRendererOutputSize(this->render,&scr_w,NULL);
		// output score
		for (int i = 0 ; i < 10 ; i ++){
			int i_score =  this->scoreboard[i].score;
			std::string i_name =  this->scoreboard[i].name;

			std::stringstream ss;
			ss << i_score << " - " << i_name;
			std::string s = ss.str();
			char * tab2 = new char [s.length()+1];
			strcpy (tab2, s.c_str());

			SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font_subtitle,tab2,{255,255,255,255});
			SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
			SDL_Rect txt_score_dest;
			SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
			txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
			txt_score_dest.y = 300 + (i*40);
			SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
			SDL_DestroyTexture(tx_score);
			SDL_FreeSurface(temp_surface);
		}

		SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font_subtitle,"back",{0,255,255,255});
		SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
		SDL_Rect txt_score_dest;
		SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
		txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
		txt_score_dest.y = 800;

		SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
		SDL_DestroyTexture(tx_score);
		SDL_FreeSurface(temp_surface);
}

void SnakeMenu::registerScore(int score){
	Score temp = {score , this->name};
	for (int i = 0 ; i < 10 ; i ++){
		if (this->scoreboard[i].score < temp.score){
			this->scoreboard.insert(this->scoreboard.begin()+i,temp);
			break;
		}
	}
	this->writeScores();
	// save score



}

void SnakeMenu::goToTitle(){
	this->currentFrame = frame_title;
	this->selectedItem = 0;

	Mix_PlayMusic(music,-1);
}

void SnakeMenu::renderNameInput()
{
	int scr_w;
	SDL_GetRendererOutputSize(this->render,&scr_w,NULL);
	SDL_Surface *temp_surface = TTF_RenderText_Blended(this->font_text,"enter playername:",{255,255,255,255});
	SDL_Texture *tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_Rect txt_score_dest;
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 400;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	SDL_Color col[3] = {{255,255,255,255},{255,255,255,255},{255,255,255,255}};

	col[this->selectedItem] = {0,255,255,255};


	std::stringstream ss;
	ss << this->name << this->curs_char ;
	std::string s = ss.str();


	char * tempname = new char [s.length()+1];
	strcpy (tempname, s.c_str());
	temp_surface = TTF_RenderText_Blended(this->font_subtitle,tempname,col[0]);
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 450;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	temp_surface = TTF_RenderText_Blended(this->font_subtitle,"continue",col[1]);
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 600;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);

	temp_surface = TTF_RenderText_Blended(this->font_subtitle,"back",col[2]);
	tx_score = SDL_CreateTextureFromSurface(this->render, temp_surface);
	SDL_QueryTexture(tx_score, NULL, NULL, &txt_score_dest.w, &txt_score_dest.h);
	txt_score_dest.x = (scr_w / 2)-(txt_score_dest.w / 2);
	txt_score_dest.y = 650;
	SDL_RenderCopy(this->render, tx_score, NULL, &txt_score_dest);
	SDL_DestroyTexture(tx_score);
	SDL_FreeSurface(temp_surface);
}


void SnakeMenu::readScores()
{
	std::ifstream openFile;
	openFile.open(saveFiles);
	std::string line;
	if (openFile.is_open()){

		std::getline (openFile,line) ;
		line.pop_back();
		this->name = line;

		while ( std::getline (openFile,line) )
		{
			line.pop_back();
			std::size_t found = line.find(",");
			std::string tmp_name = line.substr(found + 1);
			int tmp_score_int;
			std::istringstream iss (line.substr(0,found));
			iss >> tmp_score_int;

			std::cout << tmp_score_int << " - " << tmp_name << '\n';
			Score dummyscore = {tmp_score_int,tmp_name};
			this->scoreboard.push_back(dummyscore);
		}
		openFile.close();
	}
	else{
		Score dummyscore = {0,"player"};
		for (int i = 0 ; i < 10 ; i ++){
			this->scoreboard.push_back(dummyscore);
		}
	}
}

void SnakeMenu::writeScores()
{
	std::ofstream saveFile;
	saveFile.open(saveFiles, std::ofstream::out | std::ofstream::trunc);
	saveFile << this->name << ";\n";
	for (int i = 0 ; i < 10 ; i ++){
		saveFile << this->scoreboard[i].score <<","<< this->scoreboard[i].name << ";\n";
	}
	saveFile.close();
}
