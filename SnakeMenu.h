/*
 * SnakeMenu.h
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */

#ifndef SNAKEMENU_H_
#define SNAKEMENU_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#define PI 3.14159265
enum {
	menukey_up,
	menukey_down,
	menukey_left,
	menukey_right,
	menukey_back,
	menukey_enter
};

enum {
	frame_splash,
	frame_title,
	frame_title_fadeout_exit,
	frame_title_fadeout_start,
	frame_can_exit,
	frame_can_start,
	frame_scoreboard,
	frame_nameinput
};

enum {
	btn_start,
	btn_score,
	btn_exit
};

struct Score {
	int score;
	std::string name;
};

typedef Uint8 Menukey;

class SnakeMenu {
private:
	SDL_Renderer *render;
	SDL_Texture *stars;
	SDL_Texture *moon;
	SDL_Texture *earth;

	SDL_Texture *splash;

	Mix_Chunk *snd_select;
	Mix_Chunk *snd_enter;

	Mix_Music *music;

	TTF_Font *font_title;
	TTF_Font *font_subtitle;
	TTF_Font *font_text;

	std::vector<Score> scoreboard;

	int currentFrame;

	int selectedItem;

	int splashTimeout;
	int splashFade;

	int skyBoxPos = 0;

	double earthPhase;
	Uint32 lastAnimFrame = 0;
	Uint32 lastCurs = 0;
	Uint32 lastBlend = 0;

	char curs_char;


	void readScores();
	void writeScores();
public:
	std::string name;

	SnakeMenu(SDL_Renderer *render);
	virtual ~SnakeMenu();

	void pressKey(Menukey key);
	int renderFrame();
	void registerScore(int score);
	void renderMenuButtons();
	void renderMenuScore();

	void renderNameInput();

	void goToTitle();
};

#endif /* SNAKEMENU_H_ */
