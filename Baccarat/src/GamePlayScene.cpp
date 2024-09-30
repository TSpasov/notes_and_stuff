#include "GamePlayScene.h"
#include <SDL_image.h>

GamePlayScene::GamePlayScene(SDL_Renderer* renderer) {
    // Load background texture
    SDL_Surface* background = IMG_Load("data/cards.png");
    m_cardsTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);

    // Initialize game objects and UI elements
}

GamePlayScene::~GamePlayScene() {
    SDL_DestroyTexture(m_backgroundTexture);
    // Clean up other resources
}

void GamePlayScene::Draw(SDL_Renderer* renderer) {
    // Draw cards texture
    SDL_RenderCopy(renderer, m_cardsTexture, &m_cardsSrcRect, &m_cardsDstRect);
    
    // Draw game objects and UI elements
}