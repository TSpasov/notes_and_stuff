
#include "State.h"
#include <iostream>
#include <ostream>

using namespace  std::chrono_literals;

// Base state
void State::AcceptConnection(StateMachine& stateMachine)
{

}

void State::ConnectionRemoved(StateMachine& stateMachine)
{
    stateMachine.ChangeState("Idle");
}

void State::Recieve(StateMachine& stateMachine, const std::string &msg)
{
    std::cout << " Recieve from  server " << msg << std::endl;
}
// Base state

// State Idle
void Idle::Enter(StateMachine& stateMachine)
{

    stateMachine.m_tm->ScheduleTimer(this, "CheckForConnection", [&]()
    {
        if(stateMachine.m_nt->TryToConnect())
        {

            stateMachine.m_tm->StopTimer(this, "CheckForConnection");

        }
    }, 1000ms, -1);
}

void Idle::ChangeState(StateMachine& stateMachine)
{

}

void Idle::Exit(StateMachine& stateMachine)
{

}
// State Idle

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
