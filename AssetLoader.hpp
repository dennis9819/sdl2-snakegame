/*
 * AssetLoader.hpp
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */



#ifndef ASSETLOADER_HPP_
#define ASSETLOADER_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>



namespace AssetLoader {
typedef std::string rootPath;
rootPath AL_DefineRootPath(std::string path);

TTF_Font *AL_LoadFont(rootPath root,std::string assetName, int fontSize);
Mix_Music *AL_LoadMusic(rootPath root,std::string assetName);
SDL_Texture *AL_LoadTexture(rootPath root,std::string assetName , SDL_Renderer * renderer);
Mix_Chunk *AL_LoadChunk(rootPath root,std::string assetName);

std::string AL_AssetURL(rootPath root,std::string assetName);
}


#endif /* ASSETLOADER_HPP_ */
