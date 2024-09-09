#ifndef DECKOFCARDS_H
#define DECKOFCARDS_H

#include <vector>

#include "Card.h"

class DeckOfCards
{
public:
    void Init();
    void Shuffle();
    Card GetCard();

private:
    std::vector<Card> m_deck;
};

#endif // DECKOFCARDS_H
