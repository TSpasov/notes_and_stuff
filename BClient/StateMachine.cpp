
#include "StateMachine.h"
#include "State.h"
#include <iostream>

StateMachine::StateMachine(std::shared_ptr<Timer> tm,
                           std::shared_ptr<NetworkMgr> nt,
                           std::shared_ptr<Context> cntx):
    m_tm(tm),
    m_nt(nt),
    m_cntx(cntx)
{
    m_nt->OnAcceptConnection([&]()
    {
        currentState->AcceptConnection(*this);
    });

    m_nt->OnRemoveConnection([&]()
    {
        currentState->ConnectionRemoved(*this);
    });

    m_nt->OnRecieve([&](const std::string& msg)
    {
        currentState->Recieve(*this, msg);
    });
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
