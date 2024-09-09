#ifndef DEALER_H
#define DEALER_H

#include "Card.h"
#include "deckofcards.h"

class Dealer
{
public:
    enum Outcome { PLAYER_WIN, BANKER_WIN, TIE };

    void DealHands();
    Outcome InferOutcome();
    Hand GetPlayerHand() const;
    Hand GetBankerHand() const;

    Hand m_playerHand;
    Hand m_bankerHand;
    DeckOfCards m_deck;
};

#endif // DEALER_H
