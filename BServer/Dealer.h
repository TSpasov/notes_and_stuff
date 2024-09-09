#ifndef DEALER_H
#define DEALER_H

#include "Card.h"
#include "deckofcards.h"

class Dealer
{
public:
    void DealHands();
    Outcome InferOutcome();
    Hand GetPlayerHand() const;
    Hand GetBankerHand() const;

    Hand m_playerHand;
    Hand m_bankerHand;
    DeckOfCards m_deck;
};

#endif // DEALER_H
