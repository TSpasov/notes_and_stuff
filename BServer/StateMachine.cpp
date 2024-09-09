
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
    m_nt->OnAcceptConnection([&](int clID)
    {
        currentState->AcceptConnection(*this, clID);
    });

    m_nt->OnRemoveConnection([&](int clID)
    {
        currentState->ConnectionRemoved(*this, clID);
    });

    m_nt->OnRecieve([&](int clID, const std::string& msg)
    {
        currentState->Recieve(*this, clID, msg);
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
    std::cout << m_cntx->parser.StateChange(stateName) << std::endl;
    m_nt->SendToAll(m_cntx->parser.StateChange(stateName));

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
