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

#include "Context.h"

using namespace std;

class Game {
public:
    Game(Context* ct);
    virtual ~Game();

    auto Init(const char*, int, int, int, int, int) -> bool;
    void Draw();
    void Update();
    auto Running() -> bool;

    void CreateScene(const std::string& sceneId);

private:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture* m_BackGroundTexture;


    SDL_Event e;
    bool m_bRunning;

    std::string text;
    char *composition{};
    Sint32 cursor;
    Sint32 selection_len;

    Context *ctx;
//    GameScene m_scene;

};

#endif /* GAME_H_ */
