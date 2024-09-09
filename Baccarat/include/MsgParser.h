#ifndef MSGPARSER_H
#define MSGPARSER_H

#include <iostream>
#include <vector>
#include <string>
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
