
#include <memory>

#include "Game.h"

#include "NetworkMgr.h"
#include "Timer.h"
#include "StateMachine.h"
#include "State.h"
#include "Context.h"
#include "MsgParser.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// our Game object
Game* g_game = 0;
int main(int argc, char* argv[]) {





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

   // g_game = new Game();
   // g_game->Init("Baccarat", 170, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    //main loop
    while (true /*g_game->Running()*/)
    {
        nMgr->Process();
        tm->Process();
      //  g_game->Update();
      //  g_game->Draw();

    } //end main loop

    delete g_game;

    return 0;
}
