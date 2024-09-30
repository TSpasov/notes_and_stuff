#include "Dealing.h"

// State Dealing
void Dealing::Enter(StateMachine& stateMachine)
{
     stateMachine.m_cntx->CreateScene("GamePlay");
}

void Dealing::ChangeState(StateMachine &)
{


}

void Dealing::Exit(StateMachine& stateMachine)
{

}
// State Dealing