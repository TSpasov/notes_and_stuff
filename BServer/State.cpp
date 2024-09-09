
#include "State.h"
#include <iostream>
#include <ostream>

using namespace  std::chrono_literals;

// Base state
void State::AcceptConnection(StateMachine& stateMachine, int clID)
{

}

void State::ConnectionRemoved(StateMachine& stateMachine, int clID)
{

}

void State::Recieve(StateMachine& stateMachine, int clID, const std::string &msg)
{
    std::cout << " Recieve from " << clID << " " << msg << std::endl;
}
// Base state

// State Idle
void Idle::Enter(StateMachine& stateMachine)
{
    stateMachine.m_tm->ScheduleTimer(this, "CheckForConnection", [&]()
    {
         if(stateMachine.m_nt->HasConnections())
         {
            stateMachine.ChangeState("Betting");
            stateMachine.m_tm->StopTimer(this, "CheckForConnection");
         }
    }, 1000ms, -1);
}

void Idle::ChangeState(StateMachine& stateMachine)
{

}

void Idle::Exit(StateMachine& stateMachine)
{
   // stateMachine.m_tm->StopTimer(this, "CheckForConnection");
}
// State Idle

// State Betting
void Betting::Enter(StateMachine& stateMachine)
{
    stateMachine.m_tm->ScheduleTimer(this, "WaitForBets", [&](){
            stateMachine.ChangeState("Dealing");
        }, 3000ms, 1);
}

void Betting::ChangeState(StateMachine  &stateMachine)
{

}

void Betting::Exit(StateMachine& stateMachine)
{
}
// State Betting

// State Dealing
void Dealing::Enter(StateMachine& stateMachine)
{
    stateMachine.m_tm->ScheduleTimer(this, "DealCards", [&]()
    {
        if(stateMachine.m_nt->HasConnections())
        {
            stateMachine.ChangeState("Betting");
        }
        else
        {
            stateMachine.ChangeState("Idle");
        }
    }, 2000ms, 1);

   stateMachine.m_cntx->m_dealer.DealHands();
   Outcome outcome = stateMachine.m_cntx->m_dealer.InferOutcome();
   Hand player = stateMachine.m_cntx->m_dealer.GetPlayerHand();
   Hand banker = stateMachine.m_cntx->m_dealer.GetBankerHand();

   std::cout << stateMachine.m_cntx->parser.DealtCards("player_hand", player) << std::endl;
   stateMachine.m_nt->SendToAll(stateMachine.m_cntx->parser.DealtCards("player_hand", player));

   std::cout << stateMachine.m_cntx->parser.DealtCards("banker_hand", banker) << std::endl;
   stateMachine.m_nt->SendToAll(stateMachine.m_cntx->parser.DealtCards("banker_hand", banker));

   std::cout << stateMachine.m_cntx->parser.RoundOutcome(outcome) << std::endl;
   stateMachine.m_nt->SendToAll(stateMachine.m_cntx->parser.RoundOutcome(outcome));


}

void Dealing::ChangeState(StateMachine &)
{


}

void Dealing::Exit(StateMachine& stateMachine)
{

}
// State Dealing

void Error::Enter(StateMachine& stateMachine)
{
    // stateMachine.m_nt->SendToAll();
}

void Error::ChangeState(StateMachine &)
{

}

void Error::Exit(StateMachine &)
{

}
