#pragma once

#include "State.h"

 class Betting : public State
 {

public:
    
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };

