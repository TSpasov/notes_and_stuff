#ifndef DEALER_H
#define DEALER_H

#include "Card.h"
#include "deckofcards.h"
enum Outcome { PLAYER_WIN, BANKER_WIN, TIE };

class Dealer
{
public:

    void DealHands();
    auto InferOutcome() -> Outcome;
    auto GetPlayerHand() const -> Hand;
    auto GetBankerHand() const -> Hand;

    Hand m_playerHand;
    Hand m_bankerHand;
    DeckOfCards m_deck;
};

#endif // DEALER_H
