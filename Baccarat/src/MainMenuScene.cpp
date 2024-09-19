#include "MainMenuScene.h"
#include <SDL.h>
#include <SDL_image.h>

MainMenuScene::MainMenuScene(SDL_Renderer* renderer) {
    // Load background texture
    SDL_Surface* background = IMG_Load("data/main_menu_bg.png");
    m_backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);

    // Initialize other elements (e.g., buttons, text)
}

MainMenuScene::~MainMenuScene() {
    SDL_DestroyTexture(m_backgroundTexture);
    // Clean up other resources
}

void MainMenuScene::Draw(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, m_backgroundTexture, nullptr, nullptr);
    // Draw other menu elements
}