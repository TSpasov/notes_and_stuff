#ifndef CARD_H
#define CARD_H

#include <vector>

enum Outcome { PLAYER_WIN, BANKER_WIN, TIE };

struct Card
{
    int face;
    int suit;
};

struct Hand
{
    std::vector<Card> cards;

    int total() const
    {
        int sum = 0;
        for (const auto& card : cards)
        {
            int cardValue = (card.face > 9) ? 0 : card.face; // Face cards are worth 0
            sum += cardValue;
        }
        return sum % 10; // Baccarat uses modulo 10 for hand values
    }
};


#endif // CARD_H
