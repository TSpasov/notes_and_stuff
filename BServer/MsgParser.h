#ifndef MSGPARSER_H
#define MSGPARSER_H

#include <string>
#include "Card.h"

class MsgParser
{
public:

    std::string StateChange(const std::string& msg);
    std::string DealtCards(const std::string& role, Hand& hand);
    std::string RoundOutcome(Outcome outcome);
};

#endif // MSGPARSER_H
