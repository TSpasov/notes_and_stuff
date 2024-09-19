#pragma once

#include "Scene.h"
#include <SDL2/SDL.h>

class MainMenuScene : public Scene {
public:
    MainMenuScene(SDL_Renderer* renderer);
    ~MainMenuScene() override;

    void Draw(SDL_Renderer* renderer) override;

private:
    SDL_Texture* m_backgroundTexture;
    // Add other necessary member variables (e.g., buttons, text)
};
