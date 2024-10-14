#include <iostream>
#include <string>
#include <fstream>
using namespace std;
// #include "Logger.h"


int main() {

    ofstream log("../../logger.txt");
    std::string command;

    std::cout << "begin -> test_1.2" << std::endl;

    log << "begin -> test_1.2" << std::endl;

    while (true)
    {
        std::cout << "Enter command: start, stop or exit: ";

        std::getline(std::cin, command);

        // start block
        // log <-> command
        // start condition

        // std::cout << "[[ start condition -> command ";
        std::cout << "event -> " << command << std::endl;
        log << "event -> " << command << std::endl;


        if (command == "start")
        {
           // log -> action
          std::cout << "action -> start" << std::endl;
          log << "action -> start" << std::endl;
        }
        else if (command == "stop")
        {
           // log -> action
            std::cout << "action -> stop" << std::endl;
            std::cout << "action -> reset_values" << std::endl;

             log << "action -> stop" << std::endl;
             log << "action -> reset_values" << std::endl;
        }
        else if (command == "exit")
        {
            // log -> action
            break;
        }
        else
        {
            std::cout << "action -> invalid" << std::endl;
            // log -> action
            std::cout << "Invalid command. Please try again." << std::endl;

            log << "action -> invalid" << std::endl;
        }
    }

    // std::cout << "]] end condition -> command ";
     std::cout << "action -> exit" << std::endl;

     std::cout << "end -> test_1.2" << std::endl;

     log << "action -> exit" << std::endl;

     log << "end -> test_1.2" << std::endl;


     log.close();

    return 0;
}
