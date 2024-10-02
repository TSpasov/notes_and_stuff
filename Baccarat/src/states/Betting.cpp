#include "Betting.h"
#include "../Game.h"  // Adjust the path as necessary to correctly include Game.h

// State Betting

void Betting::Enter(StateMachine& stateMachine)
{
    stateMachine.m_cntx->CreateScene("BettingScene");

}

void Betting::ChangeState(StateMachine  &stateMachine)
{

}

void Betting::Exit(StateMachine& stateMachine)
{
}
// State Betting