#ifndef SATEMACHINE_H
#define SATEMACHINE_H

#include <string>
#include <memory>
#include <unordered_map>

#include "NetworkMgr.h"
#include "Timer.h"
#include "Context.h"

class State;

class StateMachine {

public:
    StateMachine(std::shared_ptr<Timer> tm,
                 std::shared_ptr<NetworkMgr> nt,
                 std::shared_ptr<Context> cntx);

    ~StateMachine();

    void Update();

    void ChangeState(const std::string& stateName);

    void AddState(const std::string& stateName, State* state)
    {
        states[stateName] = state;
    }

    void RemoveState(const std::string& stateName);

    State* GetCurrentState() const
    {
        return currentState;
    }

    std::shared_ptr<Timer> m_tm;
    std::shared_ptr<NetworkMgr> m_nt;
    std::shared_ptr<Context> m_cntx;
private:
    State* currentState = nullptr;
    std::unordered_map<std::string, State*> states;

};

#endif // SATEMACHINE_H
