#ifndef CARD_H
#define CARD_H

#include <vector>


struct Card
{
    int face;
    int suit;
};

struct Hand
{
    std::vector<Card> cards;

    auto total() const -> int
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
