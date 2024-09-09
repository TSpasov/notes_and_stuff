#include "Game.h"
#include <iostream>
#include <ostream>
#include  "SDL_keyboard.h"
//static objects

Game::Game()
{

    m_bRunning = true;
    m_pRenderer = NULL;
    m_pWindow = NULL;
    m_BackGroundTexture = 0;

}

Game::~Game() {
    // TODO Auto-generated destructor stub
}

//here things will be initialized ******************************************************************************

//init the window
bool Game::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow != 0) // window init success
    {
        m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
        if (m_pRenderer != 0) // renderer init success
        {
            SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

        }
        else
        {
            return false; // renderer init fail
        }
    }
    else
    {
        return false; // window init fail
    }

    m_bRunning = true; // everything inited successfully,

        SDL_Surface* background = IMG_Load("data/Baccarat_BG3-1.png");
    m_BackGroundTexture = SDL_CreateTextureFromSurface(m_pRenderer, background);

    SDL_FreeSurface(background);

    return true;

}/*end of Init*/


void Game::Update() {

    SDL_StartTextInput();

    // SDL_StopTextInput();
    SDL_Event event;
    //close windows
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:

            m_bRunning = false;
            break;

        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
            {
                text.pop_back();
            }

            if(event.key.keysym.sym == SDLK_RETURN)
            {
                // validate text
            }
            break;

        case SDL_TEXTINPUT:
            /* Add new text onto the end of our text */
            text += event.text.text;
            break;

        default:
            break;
        }
    }
}

void Game::Draw()
{
    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_BackGroundTexture, 0, 0);
    SDL_RenderPresent(m_pRenderer);
}/*end of Draw*/


bool Game::Running() {
    return m_bRunning;
}
