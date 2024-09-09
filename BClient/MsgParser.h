#ifndef MSGPARSER_H
#define MSGPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include "../tiny/tinyxml2/tinyxml2.h"
class Card {
public:
    int face;
    int suit;
};
class MsgParser
{
public:
    MsgParser();

    void Parse();

};



#endif // MSGPARSER_H
