#include "Idle.h"

// State Idle
void Idle::Enter(StateMachine& stateMachine)
{

    stateMachine.m_cntx->m_Game->CreateScene("inital_scene");
}

void Idle::ChangeState(StateMachine& stateMachine)
{

}

void Idle::Exit(StateMachine& stateMachine)
{

}
// State Idle
