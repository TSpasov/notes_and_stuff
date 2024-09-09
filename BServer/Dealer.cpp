#include "Dealer.h"


void Dealer::DealHands()
{
    m_playerHand.cards.clear();
    m_bankerHand.cards.clear();

    m_playerHand.cards.push_back(m_deck.GetCard());
    m_bankerHand.cards.push_back(m_deck.GetCard());
    m_playerHand.cards.push_back(m_deck.GetCard());
    m_bankerHand.cards.push_back(m_deck.GetCard());

    // If either hand has a total of 8 or 9, both stand
    if (m_playerHand.total() >= 8 || m_bankerHand.total() >= 8)
    {
        return;
    }

    // Player rules
    if (m_playerHand.total() <= 5)
    {
        m_playerHand.cards.push_back(m_deck.GetCard());
    }

    // Banker rules
    if (m_playerHand.cards.size() == 2)
    {
        // Player stands, banker's third card rule
        if (m_bankerHand.total() <= 5)
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        }
    }
    else
    {
        // Player draws a third card, banker draws based on player's third card
        Card thirdCard = m_playerHand.cards[2];
        int bankerTotal = m_bankerHand.total();
        if (bankerTotal <= 2)
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        } else if (bankerTotal == 3 && thirdCard.face != 8)
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        }
        else if (bankerTotal == 4 && (thirdCard.face >= 2 && thirdCard.face <= 7))
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        }
        else if (bankerTotal == 5 && (thirdCard.face >= 4 && thirdCard.face <= 7))
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        } else if (bankerTotal == 6 && (thirdCard.face == 6 || thirdCard.face == 7))
        {
            m_bankerHand.cards.push_back(m_deck.GetCard());
        }
    }
}

Outcome Dealer::InferOutcome()
{
    int playerTotal = m_playerHand.total();
    int bankerTotal = m_bankerHand.total();

    // Determine the outcome based on the totals
    if (playerTotal > bankerTotal)
    {
        return PLAYER_WIN;
    } else if (bankerTotal > playerTotal)
    {
        return BANKER_WIN;
    } else
    {
        return TIE;
    }
}

Hand Dealer::GetPlayerHand() const
{
    return m_playerHand;
}

Hand Dealer::GetBankerHand() const
{
    return m_bankerHand;
}
