#include "deckofcards.h"

#include <ctime>
#include <algorithm>
#include <random>
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
        for(int j = 0; j < CARDS_IN_DECK; ++j)
        {
            card.face = j % 13;
            card.suit = j / 13;
            m_deck.push_back(card);
        }
    }
}

void DeckOfCards::Shuffle()
{
    srand(time(nullptr));
    shuffle ( m_deck.begin(), m_deck.end() , std::mt19937(std::random_device()()));
}

auto DeckOfCards::GetCard() -> Card
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
