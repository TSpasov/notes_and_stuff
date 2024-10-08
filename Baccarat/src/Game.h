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

#include "Scene.h"
#include "MainMenuScene.h"
#include "GamePlayScene.h"
#include "BettingScene.h"
#include <memory>
#include "Context.h"

using namespace std;

class Game {
public:
    Game(Context* ct);
    virtual ~Game();

    auto Init(const char*, int, int, int, int, int) -> bool;
    auto Draw() -> void;
    auto Update() -> void;
    auto Running() -> bool;
    auto CreateScene(const std::string& sceneId) -> void;

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
    std::unique_ptr<Scene> m_pCurrentScene;  // Added scene pointer
    };

#endif /* GAME_H_ */
