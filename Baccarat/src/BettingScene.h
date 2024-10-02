#pragma once
#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class BettingScene : public Scene {
public:
    BettingScene(SDL_Renderer* renderer);
    ~BettingScene() override;

    void Draw(SDL_Renderer* renderer) override;
    void HandleEvent(const SDL_Event& event) override;

private:
    struct Button {
        SDL_Rect rect;
        std::string text;
        bool isHovered;
        bool isPressed;
    };
    std::vector<Button> m_buttons;

      void DrawButton(SDL_Renderer* renderer, const Button& button);

    SDL_Texture* m_backgroundTexture;
       TTF_Font* m_font;

    // Add other necessary member variables (e.g., buttons, text)
};
