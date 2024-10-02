#include "BettingScene.h"

BettingScene::BettingScene(SDL_Renderer* renderer) {
    // Load background texture
    TTF_Init();
    SDL_Surface* surface = IMG_Load("data/Baccarat_BG3-1.png");
    if (surface == nullptr) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
    } else {
        m_backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

        // Load font
    m_font = TTF_OpenFont("data/myriad-web-pro.ttf", 24);
    if (m_font == nullptr) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }

    m_buttons.push_back({SDL_Rect{100, 400, 200, 50}, "Bet 10", false, false});
    m_buttons.push_back({SDL_Rect{100, 460, 200, 50}, "Bet 50", false, false});
    m_buttons.push_back({SDL_Rect{100, 520, 200, 50}, "Bet 100", false, false});
    
}


BettingScene::~BettingScene() {
    if (m_backgroundTexture != nullptr) {
        SDL_DestroyTexture(m_backgroundTexture);
    }

    if (m_font != nullptr) {
        TTF_CloseFont(m_font);
    }
    TTF_Quit();
}

void BettingScene::Draw(SDL_Renderer* renderer) {
    // Draw background
    // SDL_RenderCopy(renderer, m_backgroundTexture, nullptr, nullptr);

    // Draw betting UI elements (buttons, text, etc.)
      for (const auto& button : m_buttons) {
            DrawButton(renderer, button);
        }
    // DrawText(renderer, "Current Bet: $0", 400, 100, 200, 50);

}

void BettingScene::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (auto& button : m_buttons) {
            SDL_Point point{mouseX, mouseY};
            bool isInside = SDL_PointInRect(&point, &button.rect);
            
            button.isHovered = isInside;
            
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (isInside) {
                    // Deselect all buttons
                    for (auto& otherButton : m_buttons) {
                        otherButton.isSelected = false;
                    }
                    // Select the clicked button
                    button.isSelected = true;
                    HandleButtonClick(button.text);
                }
            }
        }
    }
}

void BettingScene::DrawButton(SDL_Renderer* renderer, const Button& button) {
    SDL_Rect buttonRect = button.rect;
    
    // Choose button color based on state
    SDL_Color bgColor = button.isSelected ? SDL_Color{100, 100, 255, 255} :
                        button.isHovered ? SDL_Color{180, 180, 180, 255} :
                                           SDL_Color{200, 200, 200, 255};
    
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // Dark gray color
    SDL_RenderDrawRect(renderer, &buttonRect);
    
    // Render text
    if (m_font != nullptr) {
        SDL_Color textColor = {0, 0, 0, 255};  // Black color
        SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, button.text.c_str(), textColor);
        if (textSurface != nullptr) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != nullptr) {
                SDL_Rect textRect;
                textRect.x = buttonRect.x + (buttonRect.w - textSurface->w) / 2;
                textRect.y = buttonRect.y + (buttonRect.h - textSurface->h) / 2;
                textRect.w = textSurface->w;
                textRect.h = textSurface->h;
                
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}