/*
 * snakeGame.hpp
 *
 *  Created on: Apr 11, 2019
 *      Author: dennisgunia
 */

#ifndef SNAKEGAME_HPP_
#define SNAKEGAME_HPP_

#include "global.h"

#include "AnimatedTexture.h"
#include "SnakeTextAnimation.h"
#include "SnakeTextAnimationHelper.h"

#define offset_top 96
#define offset_bottom 32
#define offset_left 32
#define offset_right 32

#define PI 3.14159265

enum {
	dir_up = 0x00,
	dir_down = 0x02,
	dir_left = 0x04,
	dir_right = 0x08,
	dir_stop = 0x10
};

struct foodtype {
	enum {
		type_tail = -1,
		type_coin_std = 0,
		type_coin_death = 1,
		type_powerup = 2
	};
	enum {
		powerup_potion = 0,
		powerup_none = -1
	};
};
struct size {
	int X;
	int Y;
	int scale;
};

struct coordinate {
	int X;
	int Y;
	bool enabled = false;
};




struct food_item {
	int X;
	int Y;
	bool enabled = false;
	int type = foodtype::type_coin_std;
	int offset = 0;
	int ttl = -1;
	int powerupType = -1;
};

#define SNAKE_BACKGROUND 0,0,0,255
#define SNAKE_BORDER 128,128,128,255
#define SNAKE_PLAYER 255,128,0,255
#define SNAKE_TAIL 255,0,0,255
#define SNAKE_FOOD 0,0,255,255

#define amount_foodtypes 3
#define amount_powerups 1

class SnakeGame {
private:
	size Size;
	SDL_Renderer *render;

	coordinate player;
	int speed;
	int score;
	int highscore;
	int coins;
	int direction;

	food_item food[500];
	int foodcount;
	coordinate tail[10000];
	int taillen;

	bool lastWasHit;

	Mix_Chunk *snd_click;
	Mix_Chunk *snd_death;
	Mix_Chunk *snd_powerup;

	SDL_Texture *stars;

	SDL_Texture *powerup[amount_powerups];

	TTF_Font *font;
	Mix_Music *Music_normal 		= NULL;
	Mix_Music *Music_powerup1 		= NULL;

	AnimatedTexture tx_food[amount_foodtypes];

	SnakeTextAnimationHelper text_animations;
	int textAnimationStep[1];

	int activePowerup;
	int countdownPowerup;
	int tailPhase;
	void shiftTailArray();
	void addTailArray();
	void drawHud();

	coordinate tmpOffset;


public:
	SnakeGame(size Size, SDL_Renderer *render , SDL_Window *window);
	virtual ~SnakeGame();

	int getTimeOut();

	void startGame();

	void setDirection(int direction);
	bool doSnakeStep();
	void drawGame();
	void generateFood();

	void doAnimation();
	void convertToCoins();

	void drawStarfield(int x, int y);

	int getScore();
};

#endif /* SNAKEGAME_HPP_ */
