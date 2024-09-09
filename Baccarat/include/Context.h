#ifndef CONTEXT_H
#define CONTEXT_H

#include "Game.h"
class Context
{
public:
    friend class State;

    Context(Game* game);

 Game* m_Game;
private:

};

#endif // CONTEXT_H
