
#include "State.h"
#include <iostream>
#include <ostream>

using namespace  std::chrono_literals;

// Base state
// Base state


// State Betting
void Betting::Enter(StateMachine& stateMachine)
{

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
