#pragma once

#include <SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void Draw(SDL_Renderer* renderer) = 0;
    virtual void HandleEvent(const SDL_Event& event) {};
};
