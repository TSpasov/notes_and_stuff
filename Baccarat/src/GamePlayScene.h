#pragma once

#include "Scene.h"
#include <SDL.h>

class GamePlayScene : public Scene {
public:
    GamePlayScene(SDL_Renderer* renderer);
    ~GamePlayScene() override;

    void Draw(SDL_Renderer* renderer) override;

private:
    SDL_Texture* m_backgroundTexture;
    // Add other necessary member variables (e.g., game objects, UI elements)
};