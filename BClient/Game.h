/*
 * Game.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Todor
 */

#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Game {
public:
	Game();
	virtual ~Game();

	bool Init(const char*, int, int, int, int, int);
	void Draw();
	void Update();
	bool Running();

private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
    SDL_Texture* m_BackGroundTexture;


	SDL_Event e;
	bool m_bRunning;

    std::string text;
    char *composition;
    Sint32 cursor;
    Sint32 selection_len;

};

#endif /* GAME_H_ */
