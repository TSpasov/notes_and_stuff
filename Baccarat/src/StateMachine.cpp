
#include "StateMachine.h"
#include "State.h"
#include <iostream>

StateMachine::StateMachine(std::shared_ptr<Timer> tm,
                           std::shared_ptr<Context> cntx):
    m_tm(tm),
    m_cntx(cntx)
{
}

StateMachine::~StateMachine()
{
    for (auto& pair : states)
    {
        delete pair.second;
    }
}

void StateMachine::Update()
{
    if (currentState != nullptr)
    {
        currentState->ChangeState(*this);
    }
}


void StateMachine::ChangeState(const std::string& stateName)
{
    std::cout << " Changin state to: " << stateName << std::endl;

    auto newState = states.find(stateName);
    if (newState != states.end())
    {
        if (currentState != nullptr)
        {
            currentState->Exit(*this);
        }

        currentState = newState->second;
        currentState->Enter(*this);
    }
}

void StateMachine::RemoveState(const std::string &stateName)
{
    auto stateToRemove = states.find(stateName);
    if (stateToRemove != states.end())
    {
        delete stateToRemove->second;
        states.erase(stateToRemove);
    }
}
