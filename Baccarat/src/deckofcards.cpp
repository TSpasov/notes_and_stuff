#include "deckofcards.h"

#include <ctime>
#include <algorithm>
namespace
{
const int CARDS_IN_DECK = 52;
const int NUM_OF_DECKS = 8;
const int CUT_CARD = 8;
}

void DeckOfCards::Init()
{
    m_deck.clear();
    Card card;

    for(int i = 0; i < NUM_OF_DECKS; ++i)
    {
        for(int i = 0; i < CARDS_IN_DECK; ++i)
        {
            card.face = i % 13;
            card.suit = i / 13;
            m_deck.push_back(card);
        }
    }
}

void DeckOfCards::Shuffle()
{
    srand(time(NULL));
    random_shuffle ( m_deck.begin(), m_deck.end() );
}

Card DeckOfCards::GetCard()
{
    if(m_deck.size() < CUT_CARD)
    {
        Init();
        Shuffle();
    }
    Card card = m_deck.back();
    m_deck.pop_back();

    return card;
}
