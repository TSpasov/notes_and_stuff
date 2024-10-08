#ifndef STATE_H
#define STATE_H

#include "StateMachine.h"

 // State base class
 class State
 {
 public:
     // state/state machine interaction
     virtual void Enter(StateMachine&) {}
     virtual void ChangeState(StateMachine&) {}
     virtual void Exit(StateMachine&) {}

     // network ops
     virtual void AcceptConnection(StateMachine&);
     virtual void ConnectionRemoved(StateMachine&);
     virtual void Recieve(StateMachine&,const std::string&);

     virtual ~State() {}
 };


 class Idle : public State
 {
 public:
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };


 class Betting : public State
 {
 public:
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };


 class Dealing : public State
 {
 public:
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };


 class Error : public State
 {
 public:
     void Enter(StateMachine&) override;

     void ChangeState(StateMachine&) override;

     void Exit(StateMachine&) override;
 };


#endif // STATE_H
