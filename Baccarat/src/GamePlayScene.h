#pragma once
#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>

class GamePlayScene : public Scene {
public:
    GamePlayScene(SDL_Renderer* renderer);
    ~GamePlayScene() override;

    void Draw(SDL_Renderer* renderer) override;

private:
    SDL_Texture* m_backgroundTexture;
    SDL_Texture* m_cardsTexture;
    SDL_Rect m_cardsSrcRect = {0,0, 100, 100};
    SDL_Rect m_cardsDstRect = {0,0, 100, 100};
    // Add other necessary member variables (e.g., game objects, UI elements)
};