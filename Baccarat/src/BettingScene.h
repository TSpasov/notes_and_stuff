#pragma once
#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

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
        bool isSelected;
    };
    std::vector<Button> m_buttons;

      void DrawButton(SDL_Renderer* renderer, const Button& button);
          void HandleButtonClick(const std::string& buttonText) {
        // Implement button click actions here
        SDL_Log("Button clicked: %s", buttonText.c_str());
        std::cout << "Button clicked: "<< buttonText << std::endl;
    }

    SDL_Texture* m_backgroundTexture;
       TTF_Font* m_font;

    // Add other necessary member variables (e.g., buttons, text)
};
