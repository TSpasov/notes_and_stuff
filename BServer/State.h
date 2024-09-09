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
     virtual void AcceptConnection(StateMachine&, int clID);
     virtual void ConnectionRemoved(StateMachine&, int clID);
     virtual void Recieve(StateMachine&, int clID, const std::string& msg);

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
