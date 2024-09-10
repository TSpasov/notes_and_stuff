#ifndef IDLE_H
#define IDLE_H

#include "State.h"

class Idle : public State
{
public:
    void Enter(StateMachine&) override;

    void ChangeState(StateMachine&) override;

    void Exit(StateMachine&) override;
};

#endif // IDLE_H
