#include <math.h>
#include "SnakeGame.hpp"
#include "SnakeMenu.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "global.h"

SDL_Window   *Window;
SDL_Renderer *Renderer;
SDL_Surface* screenSurface = NULL;
Mix_Music *Music 		= NULL;


void exit_(void)
{

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);

	SDL_Quit();

}

void init(void)
{


	SDL_Init(SDL_INIT_EVERYTHING);

	Window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN);
	if(TTF_Init()==-1) {
	    printf("TTF_Init: %s\n", TTF_GetError());
	    exit(2);
	}
	SDL_SetWindowPosition(Window,0,0);
	SDL_SetWindowSize(Window,200,200);
	SDL_SetWindowTitle(Window, "Schlange");
	SDL_ShowWindow(Window);
	SDL_SetWindowFullscreen(Window,SDL_WINDOW_FULLSCREEN_DESKTOP);

	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

	Mix_VolumeMusic(60);

	//END 	ICON
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");


}


bool alive = true;


Uint32 ticklastFrame = 0;
Uint32 ticklastAnim = 0;
bool running = true;

int mode = 0;



void doTitle(SnakeGame *sg,SnakeMenu *sm){

	//BEGIN EVENT LOOP
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running =false;
		}
		else if( event.type == SDL_TEXTINPUT )
		{
			sm->name += event.text.text;

		}
		else if(event.type == SDL_KEYDOWN ){
			switch(event.key.keysym.sym ){
				case SDLK_SPACE:
					sm->pressKey(menukey_enter);
					break;
				case SDLK_UP:
					sm->pressKey(menukey_up);
					break;
				case SDLK_DOWN:
					sm->pressKey(menukey_down);
					break;
				case SDLK_ESCAPE:
					running =false;
					break;
				case SDLK_BACKSPACE:
					sm->pressKey(menukey_back);
					break;

				default:
					break;
			}
		}
	}

	switch(sm->renderFrame()){
	case frame_can_exit:
		running =0;
		break;
	case frame_can_start:
		mode = 1;
		sg->startGame();
		Mix_VolumeMusic(MIX_MAX_VOLUME/2);

		break;
	default:
		break;
	}

}

void doGame(SnakeGame *sg,SnakeMenu *sm){
	Uint32 now = SDL_GetTicks();
	if (ticklastAnim + 30 < now){
		sg->doAnimation();
		ticklastAnim = now;
	}

	if (ticklastFrame + sg->getTimeOut() < now){
		if ( !sg->doSnakeStep()){
			// what happens if dead
			sm->registerScore(sg->getScore());
			sg->setDirection(dir_stop);
			alive = false;
			sg->convertToCoins();
			mode = 0;
			Mix_HookMusicFinished(NULL);
			Mix_HaltMusic();
			sm->goToTitle();
		}
		if ((rand()%30 > 22 )&& alive){sg->generateFood();};

		ticklastFrame=now;
	}


	//BEGIN EVENT LOOP
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running =0;
		}
		if(event.type == SDL_KEYDOWN ){
			switch(event.key.keysym.sym ){
				case SDLK_UP:
					sg->setDirection(dir_up);
					alive = true;
					break;
				case SDLK_LEFT:
					sg->setDirection(dir_left);
					alive = true;
					break;
				case SDLK_DOWN:
					sg->setDirection(dir_down);
					alive = true;
					break;
				case SDLK_RIGHT:
					sg->setDirection(dir_right);
					alive = true;
					break;
				case SDLK_ESCAPE:
					mode = 0;
					Mix_HookMusicFinished(NULL);
					Mix_HaltMusic();
					sm->goToTitle();
					break;

				default:
					break;
			}
		}
	}

	sg->drawGame();

}

int main(int argc, char *argv[])
{
	init();

	SDL_RenderClear(Renderer);

	size GameField;
	GameField.X = 80;
	GameField.Y = 60;
	GameField.scale = 32;
	SnakeGame sg = SnakeGame(GameField,Renderer, Window);
	SnakeMenu sm = SnakeMenu(Renderer);


	Uint32 ticklastFrame = 0;
	Uint32 ticklastAnim = 0;

	mode = 0;
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);

	Mix_VolumeMusic(MIX_MAX_VOLUME);
	while(running){
		if(mode==0){
			doTitle(&sg,&sm);
		}else{
			doGame(&sg,&sm);
		}
	};
}
