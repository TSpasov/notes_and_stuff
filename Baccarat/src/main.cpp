
#include <memory>

#include "Game.h"

#include "NetworkMgr.h"
#include "Timer.h"
#include "StateMachine.h"
#include "states/Betting.h"
#include "states/Dealing.h"
#include "states/EndRound.h"
#include "states/State.h"
#include "states/Idle.h"
#include "Context.h"
#include "MsgParser.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// our Game object
Game* g_game = nullptr;
auto main(int argc, char* argv[]) -> int {

 
    std::shared_ptr<Timer> tm = std::make_shared<Timer>();
    std::shared_ptr<Context> cntx = std::make_shared<Context>();

   
    StateMachine machine(tm, cntx);
    // register states
    machine.AddState("Idle", new Idle);
    machine.AddState("Betting", new Betting);
    machine.AddState("Dealing", new Dealing);
    machine.AddState("EndRound", new EndRound);


    g_game = new Game(cntx.get());
    g_game->Init("Baccarat", 170, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // set inital state
    machine.ChangeState("Idle");



    //main loop
    while (g_game->Running())
    {
        tm->Process();
        g_game->Update();
        g_game->Draw();

    } //end main loop

    delete g_game;

    return 0;
}
