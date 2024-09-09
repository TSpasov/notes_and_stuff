#ifndef CONTEXT_H
#define CONTEXT_H

#include "MsgParser.h"
#include "Dealer.h"

class Context
{
public:
    friend class State;

    MsgParser parser;
    Dealer m_dealer;

private:



};

#endif // CONTEXT_H
