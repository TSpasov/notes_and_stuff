#pragma once

#include "State.h"

 class EndRound : public State
 {
 public:
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };

