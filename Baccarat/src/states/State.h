#ifndef STATE_H
#define STATE_H

#include "../StateMachine.h"

 // State base class
 class State
 {
 public:
     // state/state machine interaction
     virtual void Enter(StateMachine&) {}
     virtual void ChangeState(StateMachine&) {}
     virtual void Exit(StateMachine&) {}

     virtual ~State() = default;
 };

#endif // STATE_H
