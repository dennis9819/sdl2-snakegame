/*
 * AssetLoader.cpp
 *
 *  Created on: Apr 13, 2019
 *      Author: dennisgunia
 */


#include "AssetLoader.hpp"

namespace AssetLoader {

rootPath AL_DefineRootPath(std::string path)
{
	return path;
}

TTF_Font *AL_LoadFont(rootPath root,std::string assetName, int fontSize)
{
	std::string url = AL_AssetURL(root,assetName);
	std::cout << "Load asset @ " << url << "\n";
	char * temp = new char [url.length()+1];
	strcpy (temp, url.c_str());
	TTF_Font *font = TTF_OpenFont(temp, fontSize);
	if(!font) {
		printf("Cannot load asset @ %s\n", url);
	    printf("SDL Error: %s\n", TTF_GetError());
	}
	return font;
}

Mix_Music *AL_LoadMusic(rootPath root,std::string assetName)
{
	std::string url = AL_AssetURL(root,assetName);
	std::cout << "Load asset @ " << url << "\n";
	char * temp = new char [url.length()+1];
	strcpy (temp, url.c_str());
	Mix_Music *music = Mix_LoadMUS(temp);
	if(!music) {
		printf("Cannot load asset @ %s\n", url);
		printf("SDL Error: %s\n", TTF_GetError());
	}
	return music;
}
Mix_Chunk *AL_LoadChunk(rootPath root,std::string assetName)
{
	std::string url = AL_AssetURL(root,assetName);
	std::cout << "Load asset @ " << url << "\n";
	char * temp = new char [url.length()+1];
	strcpy (temp, url.c_str());
	Mix_Chunk *music = Mix_LoadWAV(temp);
	if(!music) {
		printf("Cannot load asset @ %s\n", url);
		printf("SDL Error: %s\n", TTF_GetError());
	}
	return music;
}

SDL_Texture *AL_LoadTexture(rootPath root,std::string assetName , SDL_Renderer * renderer)
{
	std::string url = AL_AssetURL(root,assetName);
	std::cout << "Load asset @ " << url << "\n";
	char * temp = new char [url.length()+1];
	strcpy (temp, url.c_str());
	SDL_Texture *tx = IMG_LoadTexture(renderer, temp);
	if(!tx) {
		printf("Cannot load asset @ %s\n", url);
	    printf("SDL Error: %s\n", TTF_GetError());
	}
	return tx;
}

std::string AL_AssetURL(rootPath root,std::string assetName)
{
	std::stringstream ss;
	ss << root << "/" << assetName;
	return ss.str();
}

} /* namespace AssetLoader */
