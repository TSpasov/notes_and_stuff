
#include "NetworkMgr.h"
#include "Timer.h"
#include "StateMachine.h"
#include "State.h"
#include "Context.h"
#include "MsgParser.h"

int main()
{

    std::shared_ptr<Timer> tm = std::make_shared<Timer>();
    std::shared_ptr<NetworkMgr> nMgr = std::make_shared<NetworkMgr>();
    std::shared_ptr<Context> cntx = std::make_shared<Context>();

    StateMachine machine(tm, nMgr, cntx);
    // register states
    machine.AddState("Idle", new Idle());
    machine.AddState("Betting", new Betting());
    machine.AddState("Dealing", new Dealing());
    machine.AddState("Error", new Error());

    // set inital state
    machine.ChangeState("Idle");

    // process
    while (true)
    {
        nMgr->Process();
        tm->Process();
    }
    return 0;

}
